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

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../is_zero.hpp"
#include "../optional_index.hpp"
#include "../reverse.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "outer_generator.hpp"
#include "outer_index.hpp"
#include "shift_right.hpp"
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
template <class Super>
struct layer_outer_decoder : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;
    /// Configure the layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_coefficients.resize(Super::outer_segments());
    }

    /// Map to the outer decoder all pivots in the inner code representing outer
    /// code symbols.
    ///
    /// We loop from the back of the matrix and whenever we see a "hole" we try
    /// to fill it with an outer code symbol.
    ///
    /// As an example if the below represented the matrix where 'x' represents
    /// a pivot and 'o' represents an outer code symbol. We would move from
    /// right to left and for all "holes" try to map the next 'o' into it:
    ///
    ///        x
    ///         x
    ///          x
    ///           o ----+
    ///            x    |
    ///             x   |
    ///              x  |
    ///               x |
    ///                x|
    ///                 v
    ///                  x
    ///                   x
    ///                    x
    ///                     o--+
    ///                      x |
    ///                       x|
    ///                        v
    ///                         x
    ///                          x
    ///                           x
    ///                            o
    ///                             x
    ///                              x
    ///
    ///
    auto map_to_outer_symbols()
    {
        // Loop over all valid symbol indices
        detail::symbol_range pivots = to_symbol_range(Super::symbols());

        // Track the "holes" (data symbols for which we do not have a pivot). If
        // we do not have any missing data pivots we don't map any outer code
        // symbols - since they are only used when we find a hole in the inner
        // code.
        symbol_count missing{0};

        // Loop from the back - this ensure that when we meet a hole in the
        // decoding we use the closest outer code symbol
        for (symbol_index i : reverse(pivots))
        {
            symbol_type type = Super::map_to_type(i);

            if (Super::is_inner_pivot(i))
            {
                if (type == symbol_type::outer)
                {
                    if (missing > symbol_count{0})
                    {
                        optional_index outer_pivot =
                            map_inner_to_outer_symbol(i);

                        if (outer_pivot.found())
                        {
                            --missing;
                        }
                    }
                }
            }
            else
            {
                if (type == symbol_type::data)
                {
                    ++missing;
                    Super::log("index ", i.value, " missing, now ",
                               missing.value);
                }
            }
        }
    }

    /// When the inner code finds pivots that are already pivots in the outer
    /// code we have to move the outer code symbol
    auto move_outer_symbol(symbol_index pivot_index) -> optional_index
    {
        assert(Super::is_inner_pivot(pivot_index));
        assert(Super::is_outer_pivot(pivot_index));

        // Fetch the inner coefficients and subtract them
        inner_coefficients_type inner_coefficients =
            Super::inner_coefficients_data(pivot_index);
        inner_coefficients_type* outer_coefficients =
            Super::outer_coefficients_data(pivot_index);

        outer_coefficients[0] ^= inner_coefficients;

        // Subtract the symbols
        const uint8_t* inner_symbol = Super::symbol_data(pivot_index);
        uint8_t* outer_symbol = Super::outer_symbol_data(pivot_index);

        Super::vector_subtract_into(outer_symbol, inner_symbol,
                                    Super::max_symbol_bytes());

        // Remove the outer pivot - it's being replace by the inner
        outer_index outer_symbol_index = Super::remove_outer_pivot(pivot_index);

        // The offset is where the outer coefficients start - since the
        // pivot bit is not stored in the coefficient vector we increment
        // the pivot position by one to get the offset.
        symbol_index offset = pivot_index + symbol_count{1U};

        // Continue forward elimination on the outer symbol
        return forward_eliminate(outer_symbol_index, offset);
    }

    /// Map an outer symbol which is pivot in the inner code to the
    /// outer code
    auto map_inner_to_outer_symbol(symbol_index pivot_index) -> optional_index
    {
        assert(Super::is_inner_pivot(pivot_index));
        assert(!Super::is_outer_pivot(pivot_index));
        assert(Super::map_to_type(pivot_index) == symbol_type::outer);

        // The relative position of the symbol in the outer code
        outer_index outer_symbol_index = Super::to_outer_index(pivot_index);

        Super::generate_outer_coefficients(pivot_index, m_coefficients.data());

        // Add the current inner coefficients to get the actual encoding
        // vector
        inner_coefficients_type inner_coefficients =
            Super::inner_coefficients_data(pivot_index);
        m_coefficients[0] ^= inner_coefficients;

        // Copy the coefficient and symbol data from the pivot in the inner code
        // to the storage in the outer code.
        // We cannot mix the memory for the inner and outer code since this
        // would invalidate the encoding vectors
        Super::copy_from_symbol(pivot_index,
                                Super::outer_symbol_data(outer_symbol_index));
        Super::copy_into_outer_coefficients(outer_symbol_index,
                                            m_coefficients.data());

        // The coefficients offset i.e. which symbols the bits in the
        // coefficient vector refers to
        symbol_index offset = pivot_index + symbol_count{1};

        // Pass it to the outer decoder
        return forward_eliminate(outer_symbol_index, offset);
    }

private:
    /// Add a symbol to the outer code where the pivot is already determined
    /// and the offset determines the start of the coefficients
    auto forward_eliminate(outer_index outer_symbol_index, symbol_index offset)
        -> optional_index
    {
        inner_coefficients_type* coefficients =
            Super::outer_coefficients_data(outer_symbol_index);
        uint8_t* symbol = Super::outer_symbol_data(outer_symbol_index);

        while (!is_zero(coefficients, Super::outer_coefficients_bytes()))
        {
            if (offset >= Super::symbol_upper_bound())
            {
                // We may actually still have non-zero bits in the encoding
                // vector - however these are invalid so we just don't look
                // at them. One could wrap-around the outer code - but we've
                // no implemented that so far. Would be more complex to
                // implement.
                break;
            }

            if ((0b1 & coefficients[0]) == 0)
            {
                // Zero bit in the coefficient vector - nothing to do just
                // moveto next offset
                shift_right(coefficients, Super::outer_segments());
                ++offset;
                continue;
            }

            if (Super::is_inner_pivot(offset))
            {
                // Subtract the pivot already held by the inner code.

                // The inner pivot bit is not part of the coefficient vector
                // so we bit-shift down once before the XOR
                inner_coefficients_type inner_coefficients =
                    Super::inner_coefficients_data(offset);
                const uint8_t* inner_symbol = Super::symbol_data(offset);

                Super::vector_subtract_into(symbol, inner_symbol,
                                            Super::max_symbol_bytes());

                shift_right(coefficients, Super::outer_segments());
                coefficients[0] = inner_coefficients ^ coefficients[0];

                ++offset;
                continue;
            }

            if (Super::is_outer_pivot(offset))
            {
                // Subtract the pivot already held by the outer code.

                // The outer pivot bit is not part of the coefficient vector
                // so we bit-shift down once before the XOR
                shift_right(coefficients, Super::outer_segments());

                // Subtract the symbol
                const uint8_t* outer_symbol = Super::outer_symbol_data(offset);
                const inner_coefficients_type* outer_coefficients =
                    Super::outer_coefficients_data(offset);

                Super::vector_subtract_into(symbol, outer_symbol,
                                            Super::max_symbol_bytes());

                Super::vector_subtract_into(
                    reinterpret_cast<uint8_t*>(coefficients),
                    reinterpret_cast<const uint8_t*>(outer_coefficients),
                    Super::outer_coefficients_bytes());

                ++offset;
                continue;
            }

            if (Super::map_to_type(offset) == symbol_type::outer)
            {
                // The generated coefficient vector does not have the offset as
                // part of the encoding vector so we bit-shift down once before
                // the XOR
                shift_right(coefficients, Super::outer_segments());
                Super::generate_outer_coefficients(offset,
                                                   m_coefficients.data());

                Super::vector_subtract_into(
                    reinterpret_cast<uint8_t*>(coefficients),
                    reinterpret_cast<const uint8_t*>(m_coefficients.data()),
                    Super::outer_coefficients_bytes());

                ++offset;
                continue;
            }

            // We have a new pivot - we store the pivot bit seperately from
            // the remaning coefficients
            shift_right(coefficients, Super::outer_segments());

            // Store the outer code information
            Super::set_outer_pivot(outer_symbol_index, offset);

            return optional_index(true, offset);
        }

        return optional_index::not_found();
    }

private:
    /// Temporary buffer coding coefficients being generated as we map
    /// inner symbols to the outer code
    std::vector<inner_coefficients_type> m_coefficients;
};

}
}
}
}
