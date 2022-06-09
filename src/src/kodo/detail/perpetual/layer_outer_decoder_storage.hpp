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
#include <cstring>
#include <map>
#include <vector>

#include "../bitset.hpp"
#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "outer_index.hpp"
#include "symbol_type.hpp"

#include "../../version.hpp"
namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// The storage for the outer code coefficients and symbols
///
/// In principle the outer code can fill a pivot for any data symbol. If
/// a pivot is held by the outer code. We use a mapping to denote which
/// of the other code symbols corresponds to the pivot.
///
/// If a outer code symbol has not been filled the mapping will contain
/// the upper bound symbol index (i.e. a non valid index)
template <class Super>
struct layer_outer_decoder_storage : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    /// Configure that outer storage
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        symbol_count outer_symbols = Super::outer_symbols();
        std::size_t outer_segments = Super::outer_segments();

        // Resize all the internal data stuctures
        m_outer_mapping.clear();

        m_coefficients.resize(outer_symbols.value * outer_segments, 0);

        byte_count outer_block = outer_symbols * Super::max_symbol_bytes();
        m_outer_symbols.resize(outer_block.value);
    }

    /// Check if the outer code has a specific pivot
    auto is_outer_pivot(symbol_index index) const -> bool
    {
        return m_outer_mapping.find(index) != m_outer_mapping.end();
    }

    /// Set that a specific that an outer code symbol now represents a pivot
    /// in the outer code.
    void set_outer_pivot(outer_index index, symbol_index pivot_index)
    {
        assert(!Super::is_inner_pivot(pivot_index));
        assert(!is_outer_pivot(pivot_index));

        // Store the offset to the outer code symbol that the pivot refers to
        m_outer_mapping[pivot_index] = index;
    }

    /// We may have found a pivot in the inner code which takes the place
    /// of the outer pivot. In this case we unset the pivot bit and
    /// remove the mapping.
    auto remove_outer_pivot(symbol_index pivot_index) -> outer_index
    {
        assert(is_outer_pivot(pivot_index));
        auto outer_index = m_outer_mapping.at(pivot_index);
        m_outer_mapping.erase(pivot_index);
        return outer_index;
    }

    /// Return the coefficients of a symbol in the outer code
    auto outer_coefficients_data(symbol_index pivot_index)
        -> inner_coefficients_type*
    {
        assert(is_outer_pivot(pivot_index));
        return outer_coefficients_data(m_outer_mapping.at(pivot_index));
    }

    /// Return the coefficients of a symbol in the outer code
    auto outer_coefficients_data(symbol_index pivot_index) const
        -> const inner_coefficients_type*
    {
        assert(is_outer_pivot(pivot_index));
        return outer_coefficients_data(m_outer_mapping.at(pivot_index));
    }

    /// Return the coefficients of a symbol in the outer code
    auto outer_coefficients_data(outer_index index) -> inner_coefficients_type*
    {
        auto offset = Super::outer_segments() * index.value;
        return m_coefficients.data() + offset;
    }

    /// Return the coefficients of a symbol in the outer code
    auto outer_coefficients_data(outer_index index) const
        -> const inner_coefficients_type*
    {
        auto offset = Super::outer_segments() * index.value;
        return m_coefficients.data() + offset;
    }

    /// Return the rank of the outer code
    auto outer_rank() const -> symbol_count
    {
        return symbol_count{m_outer_mapping.size()};
    }

    /// Copy src into the specified symbol
    void copy_into_outer_coefficients(outer_index index,
                                      const inner_coefficients_type* src)
    {
        assert(index < Super::outer_symbols());
        assert(src != nullptr);

        std::memcpy(outer_coefficients_data(index), (const uint8_t*)src,
                    Super::outer_coefficients_bytes().value);
    }

    /// Copy src into the specified symbol
    void copy_into_outer_symbol(outer_index index, const uint8_t* src)
    {
        assert(index < Super::outer_symbols());
        assert(src != nullptr);

        std::memcpy(outer_symbol_data(index), src,
                    Super::max_symbol_bytes().value);
    }

    /// Copy the specified symbol into dst
    void copy_from_outer_symbol(symbol_index index, uint8_t* dst) const
    {
        assert(index < Super::symbols());
        assert(dst != nullptr);

        std::memcpy(dst, outer_symbol_data(index),
                    Super::max_symbol_bytes().value);
    }

    /// Return the symbol at the specified position
    auto outer_symbol_data(symbol_index index) -> uint8_t*
    {
        assert(index < Super::symbols());
        return outer_symbol_data(m_outer_mapping.at(index));
    }

    /// Return the symbol at the specified position
    auto outer_symbol_data(symbol_index index) const -> const uint8_t*
    {
        assert(index < Super::symbols());
        return outer_symbol_data(m_outer_mapping.at(index));
    }

    /// Return the symbol at the specified position
    auto outer_symbol_data(outer_index index) -> uint8_t*
    {
        assert(index.value < Super::symbol_upper_bound().value);
        assert(index < Super::outer_symbols());

        byte_count symbol_bytes = Super::max_symbol_bytes();
        return m_outer_symbols.data() + (index.value * symbol_bytes.value);
    }

    /// Return the symbol at the specified position
    auto outer_symbol_data(outer_index index) const -> const uint8_t*
    {
        assert(index.value < Super::symbol_upper_bound().value);
        assert(index < Super::outer_symbols());

        byte_count symbol_bytes = Super::max_symbol_bytes();
        return m_outer_symbols.data() + (index.value * symbol_bytes.value);
    }

private:
    /// Outer symbol index for the pivot.
    std::map<symbol_index, outer_index> m_outer_mapping;

    /// Storage for the coding coefficients for the outer code symbols
    std::vector<inner_coefficients_type> m_coefficients;

    /// Storage for the symbols in the outer code
    std::vector<uint8_t> m_outer_symbols;
};

}
}
}
}
