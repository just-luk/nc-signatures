// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
//
// Licensees holding a valid commercial license may use this project
// in accordance with the standard license agreement terms provided
// with the Software (see accompanying file LICENSE.rst or
// https://www.steinwurf.com/license), unless otherwise different
// terms and conditions are agreed in writing between Licensee and
// Steinwurf ApS in which case the license will be regulated by that
// separate written agreement.
//
// License for Non-Commercial Usage
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

#include <fifi/field/binary_math.hpp>
#include <fifi/utils.hpp>

#include "stack_elemination_decoder.hpp"

#include "../block/stack_decoder.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

template <class Super>
struct layer_decoding_algorithm : public Super
{
    using Super::m_elemination_decoder;
    using Super::m_inner_decoder;
    using Super::m_outer_decoder;

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_elemination_decoder_copied.resize(
            m_elemination_decoder.symbols().value);
        std::fill_n(m_elemination_decoder_copied.begin(),
                    m_elemination_decoder_copied.size(), false);

        m_inner_decoder_copied.resize(m_inner_decoder.symbols().value);
        std::fill_n(m_inner_decoder_copied.begin(),
                    m_inner_decoder_copied.size(), false);

        m_outer_symbol.resize(Super::symbol_bytes().value);
        m_outer_coefficients.resize(
            m_outer_decoder.max_coefficients_bytes().value);
    }

    void decode_systematic_symbol(const uint8_t* symbol, symbol_index index)
    {
        if (index < Super::symbols())
        {
            // The symbol represents original data from the inner decoder
            m_inner_decoder.decode_systematic_symbol(symbol, index);

            // This means the symbol can be immediately copied to the outer
            // decoder and marked as copied.
            m_inner_decoder_copied[index.value] = true;
            m_outer_decoder.decode_systematic_symbol(symbol, index);
        }
        else
        {
            // The symbol contains coded data in the outer field.
            auto expansion_index = index - Super::symbols();
            m_elemination_decoder.decode_systematic_symbol(symbol,
                                                           expansion_index);
        }
        check_combined_rank();
    }

    void decode_symbol(uint8_t* symbol, uint8_t* coefficients)
    {
        // We always pass the packet to the elemination decoder it
        // is responsible for eliminating the expansion.
        bool stored = m_elemination_decoder.decode_symbol(symbol, coefficients);

        if (stored)
        {
            // If the rank increased in the elemination decoder we check whether
            // we have achieved full rank.
            // We do not need to go to the inner decoder since we have
            // accumulated the additional degree of freedom contained in the
            // encoded symbol.
            check_combined_rank();
            return;
        }

        // Getting here means that the elemination decoder did not
        // increase its rank, next step is to pass the reduced
        // symbol to the inner decoder
        auto old_rank = m_inner_decoder.rank();

        m_inner_decoder.decode_symbol(symbol, coefficients);

        if (m_inner_decoder.rank() > old_rank)
        {
            // If the inner decoder increased the rank we
            // check whether we can map anything to the outer
            // decoder
            check_combined_rank();
            return;
        }
    }

    void check_combined_rank()
    {
        if (Super::inner_rank() < Super::symbols())
            return;
        // We map from inner decoder first. The is advantageous if the outer
        // encoder has produced systematic symbols. Since in that case we
        // are able to map symbols directly between the outer and inner
        // decoders.
        for (auto i : m_inner_decoder.symbol_range())
        {
            bool is_pivot = m_inner_decoder.is_symbol_pivot(i);
            bool is_copied = m_inner_decoder_copied[i.value];

            if (is_pivot && !is_copied)
            {
                /// Takes a specific symbol from the inner decoder and
                /// maps it to the outer decoder.
                map_symbol(m_inner_decoder.symbol_data(i),
                           m_inner_decoder.coefficients_data(i),
                           m_inner_decoder.symbol_range());
                m_inner_decoder_copied[i.value] = true;
            }
        }

        for (auto i : m_elemination_decoder.symbol_range())
        {
            bool is_pivot = m_elemination_decoder.is_symbol_pivot(i);
            bool is_copied = m_elemination_decoder_copied[i.value];

            if (is_pivot && !is_copied)
            {
                map_symbol(m_elemination_decoder.symbol_data(i),
                           m_elemination_decoder.coefficients_data(i),
                           m_elemination_decoder.elemination_offset_range());
                m_elemination_decoder_copied[i.value] = true;
            }
        }

        if (!m_outer_decoder.is_complete())
            return;

        for (auto i : m_outer_decoder.symbol_range())
        {
            if (m_inner_decoder.is_symbol_decoded(i))
                continue;

            // Update the corresponding vector
            uint8_t* coefficients_dest = m_inner_decoder.coefficients_data(i);

            // Zero out the memory first
            std::fill_n(coefficients_dest,
                        m_inner_decoder.max_coefficients_bytes().value, 0);
            m_inner_decoder.set_value(coefficients_dest, i, 1U);

            m_inner_decoder.copy_into_symbol(i, m_outer_decoder.symbol_data(i));
            // Mark this symbol decoded
            m_inner_decoder.set_symbol_decoded(i);
        }
    }

    /// Maps the coding coefficients from the inner code to the
    /// outer and copies the symbol for decoding in the outer code.
    void map_symbol(const uint8_t* symbol_data, const uint8_t* coefficients,
                    const detail::symbol_range& range)
    {
        // Copy the pivot symbol to a modifiable buffer, this copy
        // might not be needed - if we could guarantee
        // decoding. But we cannot right now.
        std::copy_n(symbol_data, Super::symbol_bytes().value,
                    m_outer_symbol.data());

        map_to_outer(coefficients, m_outer_coefficients.data(), range);
        m_outer_decoder.decode_symbol(m_outer_symbol.data(),
                                      m_outer_coefficients.data());
    }

    void map_to_outer(const uint8_t* inner_coefficients,
                      uint8_t* outer_coefficients,
                      const detail::symbol_range& range)
    {
        // Zero the destination coefficients buffer
        std::fill_n(outer_coefficients,
                    m_outer_decoder.max_coefficients_bytes().value, 0U);
        for (auto index : range)
        {
            if (m_inner_decoder.get_value(inner_coefficients, index) == 0)
            {
                continue;
            }

            if (index < Super::symbols())
            {
                // For the systematic part
                m_outer_decoder.set_value(outer_coefficients, index, 1U);
            }
            else
            {
                // Loop over the inner coding coefficients and create the outer
                // coding coefficients
                m_outer_decoder.vector_add_into(
                    outer_coefficients, Super::outer_coefficients(index),
                    m_outer_decoder.max_coefficients_bytes());
            }
        }
    }

private:
    /// Keeps track of which symbols have been copied from the
    /// elemination decoder to the inner decoder
    std::vector<bool> m_elemination_decoder_copied;

    /// Keeps track of which symbols have been copied from the
    /// inner decoder to the inner decoder
    std::vector<bool> m_inner_decoder_copied;

    std::vector<uint8_t> m_outer_symbol;
    std::vector<uint8_t> m_outer_coefficients;
};
}
}
}
}
