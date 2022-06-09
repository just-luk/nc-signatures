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

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../optional_index.hpp"
#include "../reverse.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// The inner decoder use the the perpetual structure of the coding
/// coefficients.
template <class Super>
struct layer_inner_decoder : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    auto decode_inner_symbol(symbol_index offset,
                             inner_coefficients_type coefficients,
                             uint8_t* symbol) -> optional_index
    {
        optional_index pivot = forward_eliminate(offset, coefficients, symbol);

        if (!pivot.found() && (coefficients != 0))
        {
            // We are wrapping around into the zero-padding
            assert(offset == Super::symbol_upper_bound());

            offset = symbol_index{0};
            pivot = forward_eliminate(offset, coefficients, symbol);
        }

        return pivot;
    }

    auto forward_eliminate(symbol_index& offset,
                           inner_coefficients_type& coefficients,
                           uint8_t* symbol) -> optional_index
    {
        while (coefficients != 0 && offset < Super::symbol_upper_bound())
        {
            if ((coefficients & 0b1) == 0)
            {
                ++offset;
                coefficients = coefficients >> 1U;
                continue;
            }

            if (Super::is_inner_pivot(offset))
            {
                inner_coefficients_type pivot_coefficients =
                    Super::inner_coefficients_data(offset);
                const uint8_t* pivot_symbol = Super::symbol_data(offset);

                Super::vector_subtract_into(symbol, pivot_symbol,
                                            Super::max_symbol_bytes());

                // The pivot coefficients does not contain the pivot bit - so
                // we perform a single right shift before we XOR the
                // coefficients
                coefficients = (coefficients >> 1U) ^ pivot_coefficients;
                ++offset;
            }
            else
            {
                // We have found a pivot. The current offset contains
                // the pivot location. We don't store the pivot bits in the
                // coefficients so we perform a single right shift before
                // storing the coefficients
                coefficients = coefficients >> 1U;
                Super::set_inner_pivot(offset, coefficients);
                Super::copy_into_symbol(offset, symbol);

                return optional_index{true, offset};
            }
        }

        return optional_index::not_found();
    }
};

}
}
}
}
