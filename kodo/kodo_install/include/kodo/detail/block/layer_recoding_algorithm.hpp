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
#include <cstdint>
#include <iostream>

#include "../fill_zero.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"
#include <fifi/utils.hpp>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
/// @brief Implements basic linear block recoder.
template <class Super>
class layer_recoding_algorithm : public Super
{
public:
    void recode_symbol(uint8_t* symbol_data, uint8_t* coefficients,
                       const uint8_t* coefficients_in) const
    {
        assert(symbol_data != 0);
        assert(coefficients != 0);
        assert(coefficients_in != 0);

        fill_zero(symbol_data, Super::symbol_bytes());
        fill_zero(coefficients, Super::max_coefficients_bytes());

        for (symbol_index index : Super::symbol_range())
        {
            auto value = Super::get_value(coefficients_in, index);

            if (value == 0)
            {
                continue;
            }

            assert(Super::is_symbol_set(index));
            const uint8_t* symbol_i = Super::symbol_data(index);
            const uint8_t* coefficients_i = Super::coefficients_data(index);
            std::cout << value << " * "
                      << fifi::get_value(fifi::finite_field::prime2325,
                                         symbol_i, 0)
                      << std::endl;

            // Did you forget to set the data on the decoder?
            assert(symbol_i != nullptr);
            assert(coefficients_i != nullptr);

            if (Super::is_binary())
            {
                Super::vector_add_into(coefficients, coefficients_i,
                                       Super::max_coefficients_bytes());
                Super::vector_add_into(symbol_data, symbol_i,
                                       Super::symbol_bytes());
            }
            else
            {
                Super::vector_multiply_add_into(
                    coefficients, coefficients_i, value,
                    Super::max_coefficients_bytes()); // coefficients =
                                                      // coefficients *
                                                      // coefficients_in
                Super::vector_multiply_add_into(
                    symbol_data, symbol_i, value,
                    Super::symbol_bytes()); // symbol = symbol * value
            }
        }
    }
};
}
}
}
}
