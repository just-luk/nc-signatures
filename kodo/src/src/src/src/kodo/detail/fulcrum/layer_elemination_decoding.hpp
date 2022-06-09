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

#include "../byte_count.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{
/// Building block for creating an "elimination decoder".
/// This layer eleminates the expansion symbols when decoding.
///
/// Doing so allows an two staged decoding where this layer will remove any
/// expansion symbols, leaving "clean" inner field-only symbols for the next
/// decoder.
template <class Super>
struct layer_elemination_decoding : public Super
{
    void decode_systematic_symbol(const uint8_t* symbol_data,
                                  symbol_index index)
    {
        if (Super::is_symbol_decoded(index))
        {
            return;
        }
        if (Super::is_symbol_partially_decoded(index))
        {
            swap_decode(symbol_data, index);
        }
        else
        {
            // Update the corresponding vector
            uint8_t* coefficients_dest = Super::coefficients_data(index);

            // Zero out the memory first
            std::fill_n(coefficients_dest,
                        Super::max_coefficients_bytes().value, 0);

            Super::set_value(coefficients_dest,
                             index + Super::elemination_offset(), 1U);

            // Mark this symbol decoded
            Super::set_symbol_decoded(index);

            Super::copy_into_symbol(index, symbol_data);
        }
    }

    bool decode_symbol(uint8_t* symbol_data, uint8_t* coefficients)
    {
        assert(coefficients != nullptr);
        assert(symbol_data != nullptr);

        for (symbol_index index : Super::symbol_range())
        {
            auto coefficient = Super::get_value(
                coefficients, index + Super::elemination_offset());

            if (!coefficient)
            {
                // The coefficient is zero
                continue;
            }

            if (!Super::is_symbol_pivot(index))
            {
                // Mark this symbol as partially decoded
                Super::set_symbol_partially_decoded(index);

                // Copy it into the symbol storage
                Super::copy_into_symbol(index, symbol_data);
                Super::copy_into_coefficients(index, coefficients);
                return true;
            }

            const uint8_t* coefficients_i = Super::coefficients_data(index);
            const uint8_t* symbol_i = Super::symbol_data(index);

            Super::vector_subtract_into(coefficients, coefficients_i,
                                        Super::max_coefficients_bytes());
            Super::vector_subtract_into(symbol_data, symbol_i,
                                        Super::symbol_bytes());
        }

        return false;
    }

private:
    void swap_decode(const uint8_t* symbol_data, symbol_index pivot_index)
    {
        assert(Super::is_symbol_partially_decoded(pivot_index));
        assert(!Super::is_symbol_decoded(pivot_index));
        assert(!Super::is_symbol_missing(pivot_index));

        Super::set_symbol_missing(pivot_index);

        auto symbol_i = Super::symbol_data(pivot_index);
        auto coefficients_i = Super::coefficients_data(pivot_index);

        assert(Super::get_value(coefficients_i,
                                pivot_index + Super::elemination_offset()) ==
               1);

        // Subtract the new pivot symbol
        Super::set_value(coefficients_i,
                         pivot_index + Super::elemination_offset(), 0U);

        Super::vector_subtract_into(symbol_i, symbol_data,
                                    Super::symbol_bytes());

        // Continue to process our new coded symbol: we know that it must
        // contain a larger pivot id than the current (unless it is
        // reduced to all zeroes).
        decode_symbol(symbol_i, coefficients_i);

        // The previous vector may still be in memory
        std::fill_n(coefficients_i, Super::max_coefficients_bytes().value, 0);

        // Stores the symbol and sets the pivot in the vector
        decode_systematic_symbol(symbol_data, pivot_index);
    }
};
}
}
}
}