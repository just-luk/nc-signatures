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

#include "../fill_zero.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
template <class Super>
class layer_encoding_algorithm : public Super
{
public:
    void encode_systematic_symbol(uint8_t* symbol_data,
                                  symbol_index index) const
    {
        assert(Super::is_symbol_set(index));
        Super::copy_from_symbol(index, symbol_data);
    }

    void encode_symbol(uint8_t* symbol_data, const uint8_t* coefficients) const
    {
        assert(symbol_data != 0);
        assert(coefficients != 0);
        fill_zero(symbol_data, Super::symbol_bytes());

        for (symbol_index index : Super::symbol_range())
        {
            auto value = Super::get_value(coefficients, index);

            if (value == 0)
            {
                continue;
            }

            assert(Super::is_symbol_set(index));
            const uint8_t* symbol_i = Super::symbol_data(index);

            // Did you forget to set the data on the encoder?
            assert(symbol_i != nullptr);

            if (Super::is_binary())
            {
                Super::vector_add_into(symbol_data, symbol_i,
                                       Super::symbol_bytes());
            }
            else
            {
                Super::vector_multiply_add_into(symbol_data, symbol_i, value,
                                                Super::symbol_bytes());
            }
        }
    }
};
}
}
}
}
