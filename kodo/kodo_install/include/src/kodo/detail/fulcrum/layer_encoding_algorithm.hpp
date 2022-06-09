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

#include <algorithm>
#include <cassert>

#include <fifi/field/binary_math.hpp>

#include "../block/stack_encoder.hpp"
#include "../symbol_index.hpp"

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
struct layer_encoding_algorithm : public Super
{
    using Super::m_inner_encoder;
    using Super::m_outer_encoder;

    void encode_symbol(uint8_t* symbol, const uint8_t* coefficients) const
    {
        m_inner_encoder.encode_symbol(symbol, coefficients);
    }

    void encode_systematic_symbol(uint8_t* symbol, symbol_index index) const
    {
        m_inner_encoder.encode_systematic_symbol(symbol, index);
    }

    /// Set the storage of the block
    void set_symbols_storage(const uint8_t* storage)
    {
        std::size_t offset = 0;
        for (symbol_index index : Super::symbol_range())
        {
            layer_encoding_algorithm::set_symbol_storage(storage + offset,
                                                         index);
            offset += Super::symbol_bytes().value;
        }
    }

    /// Set the storage of a symbol
    void set_symbol_storage(const uint8_t* storage, symbol_index index)
    {
        m_inner_encoder.set_symbol_storage(storage, index);
        m_outer_encoder.set_symbol_storage(storage, index);
        if (m_outer_encoder.rank() == m_outer_encoder.symbols())
        {
            precode();
        }
    }

    void precode()
    {
        for (symbol_index i : Super::expansion_range())
        {
            auto expansion_symbol = Super::expansion_symbol_data(i);
            auto expansion_coefficients = Super::outer_coefficients(i);
            m_outer_encoder.encode_symbol(expansion_symbol,
                                          expansion_coefficients);
            m_inner_encoder.set_symbol_storage(expansion_symbol, i);
        }
    }
};
}
}
}
}
