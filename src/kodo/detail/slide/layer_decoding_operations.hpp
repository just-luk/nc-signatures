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
#include "../in_frame.hpp"
#include "../optional_index.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "coefficients_vector.hpp"
#include "coefficients_view.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer for tracing the decoding operations
template <class Super>
struct layer_decoding_operations : public Super
{
    struct forward_substitute_result
    {
        byte_count symbol_bytes;
        optional_index pivot;
    };

    /// Iterates the encoding vector to find a pivot and subtract existing
    /// symbols.
    /// @param symbol_data the data of the encoded symbol
    /// @param symbol_bytes the number of bytes in the symbol buffer
    /// @param coefficients the encoding vector
    /// @return a struct containing the symbol pointer, symbol bytes, and
    /// an optional index representing the pivot, if found.
    /// The returned symbol pointer and bytes are the same as the input, unless
    /// the symbol was swapped during the forward substitution.
    forward_substitute_result
    forward_substitute(uint8_t* symbol_data, byte_count symbol_bytes,
                       const mutable_coefficients_view& coefficients)
    {
        auto symbol_frame =
            Super::coefficients_view_to_symbol_frame(coefficients);

        // This is the actual valid symbol indicies, note that this may be
        // different from coefficients frame since e.g. for binary and binary4
        // the frame may be larger than the range.
        auto stream_range = Super::stream_range();

        assert(symbol_frame.upper_bound() >= stream_range.upper_bound());

        optional_index pivot = optional_index::not_found();

        // Loop over all the symbol/coefficient indicies in the frame
        for (symbol_index index : symbol_frame)
        {
            // The frame of the coefficients may outside the range of the stream
            // we skip these indices
            if (!stream_range.in_range(index))
            {
                continue;
            }

            auto coefficient = Super::get_value(coefficients, index);

            if (!coefficient)
            {
                // If the coefficient is zero we move to the next index
                continue;
            }

            if (!Super::is_symbol_pivot(index))
            {
                if (!pivot.found())
                {
                    // If we have a non-zero coefficient and not already a pivot
                    // we found a pivot
                    pivot = optional_index{true, index};
                }
                continue;
            }

            // We already have a pivot here get the corresponding symbol and
            // coefficients vector and elimitate those in the incoming symbol
            auto symbol_data_i = Super::symbol_data(index);
            auto symbol_bytes_i = Super::symbol_bytes(index);
            const_coefficients_view coefficients_i =
                Super::coefficients_data(index);

            assert(symbol_data_i != symbol_data);
            assert(coefficients_i.data() != coefficients.data());

            // If the stored symbol is larger than the one we're
            // processing - swap them.
            if (symbol_bytes_i > symbol_bytes)
            {
                if (Super::is_symbol_decoded(index))
                {
                    // If symbol i is decoded, and the incoming
                    // symbol is smaller but a contains symbol i,
                    // we can derive that symbol i must at most the
                    // size of the incoming symbol.
                    symbol_bytes_i = symbol_bytes;
                    Super::update_symbol_bytes(index, symbol_bytes_i);
                }
                else
                {
                    // Increase the number of bytes in the incoming symbol. This
                    // happens when symbol i is substracted from the incoming
                    // symbol.
                    symbol_bytes = symbol_bytes_i;
                }
            }

            if (coefficient == 1U)
            {
                Super::vector_subtract_into(coefficients, coefficients_i);

                Super::vector_subtract_into(symbol_data, symbol_data_i,
                                            symbol_bytes_i);
            }
            else
            {
                Super::vector_multiply_subtract_into(
                    coefficients, coefficients_i, coefficient);

                Super::vector_multiply_subtract_into(
                    symbol_data, symbol_data_i, coefficient, symbol_bytes_i);
            }
        }

        return forward_substitute_result{symbol_bytes, pivot};
    }

    /// Backward substitute the found symbol into the existing symbols.
    /// @param pivot_index the pivot index of the symbol
    void backward_substitute(symbol_index pivot_index)
    {
        const uint8_t* symbol_data = Super::symbol_data(pivot_index);
        auto symbol_bytes = Super::symbol_bytes(pivot_index);
        auto decoded = Super::is_symbol_decoded(pivot_index);
        const const_coefficients_view coefficients =
            Super::coefficients_data(pivot_index);
        // We found a "1" that nobody else had as pivot, we now
        // substract this packet from other coded packets
        // - if they have a "1" on our pivot place
        const symbol_range range =
            symbol_range{Super::stream_lower_bound(), pivot_index};

        for (symbol_index index : range)
        {
            if (Super::is_symbol_missing(index))
            {
                // We do not have a symbol yet here
                continue;
            }

            if (Super::is_symbol_decoded(index))
            {
                // We know that we have no non-zero elements
                // outside the pivot position when a symbol is fully decoded
                continue;
            }

            // The symbol must be partially decoded
            assert(Super::is_symbol_partially_decoded(index));

            mutable_coefficients_view coefficients_i =
                Super::coefficients_data(index);
            auto coefficient = Super::get_value(coefficients_i, pivot_index);

            if (coefficient == 0)
            {
                // The coefficient is zero
                continue;
            }

            uint8_t* symbol_i = Super::symbol_data(index);
            auto symbol_bytes_i = Super::symbol_bytes(index);

            if (symbol_bytes_i < symbol_bytes)
            {
                if (decoded)
                {
                    // symbol i is smaller than the incoming symbol.
                    // In this case we deduce that the incoming
                    // symbol is infact smaller than what we think
                    // it is. This is because, since the incoming
                    // symbol is decoded, and symbol_i mixed with
                    // the incoming symbol, the incoming symbol must
                    // be equal to or less than the size of symbol
                    // i.
                    symbol_bytes = symbol_bytes_i;
                    Super::update_symbol_bytes(pivot_index, symbol_bytes);
                }
                else
                {
                    // Increase the number of bytes in symbol i. This
                    // happens when pivot symbol is substracted from symbol i.
                    Super::update_symbol_bytes(index, symbol_bytes);
                }
            }

            if (coefficient == 1U)
            {
                Super::vector_subtract_into(coefficients_i, coefficients);

                Super::vector_subtract_into(symbol_i, symbol_data,
                                            symbol_bytes);
            }
            else
            {
                Super::vector_multiply_subtract_into(coefficients_i,
                                                     coefficients, coefficient);

                Super::vector_multiply_subtract_into(symbol_i, symbol_data,
                                                     coefficient, symbol_bytes);
            }

            if (Super::is_coefficients_decoded(index))
            {
                // Mark symbol i as decoded
                Super::set_symbol_decoded(index);
            }
        }
    }

    /// Normalize the encoding vector by dividing all elements in
    /// the vector with the value at the provided index
    /// @param index the index of the symbol to normalize
    void normalize(symbol_index index)
    {
        assert(!Super::is_binary());
        assert(Super::in_stream(index));
        uint8_t* symbol_data = Super::symbol_data(index);
        auto symbol_bytes = Super::symbol_bytes(index);
        const mutable_coefficients_view& coefficients =
            Super::coefficients_data(index);
        // Get the coefficient value - which much be non-zero since it is our
        // pivot
        auto coefficient = Super::get_value(coefficients, index);
        assert(coefficient > 0);

        auto inverse = Super::invert(coefficient);

        // Update symbol and corresponding vector
        Super::vector_multiply_into(coefficients, inverse);
        Super::vector_multiply_into(symbol_data, inverse, symbol_bytes);
    }
};
}
}
}
}
