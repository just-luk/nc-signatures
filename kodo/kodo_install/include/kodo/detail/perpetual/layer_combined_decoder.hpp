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
#include <cstring>
#include <vector>

#include "../optional_index.hpp"
#include "../reverse.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "mask_coefficients.hpp"
#include "split_upper.hpp"
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
struct layer_combined_decoder : public Super
{

    using inner_coefficients_type = typename Super::inner_coefficients_type;

    /// Configure the layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        // Reset variables
        m_outer_decoding_in_progress = false;
    }

    auto decode_symbol(uint8_t* symbol, inner_coefficients_type coefficients,
                       symbol_index offset) -> optional_index
    {
        if (is_complete())
        {
            Super::log("Decoding already finished");
            // We've already finished decoding so nothing more to do
            return optional_index::not_found();
        }

        // Track the pivot we find
        optional_index pivot = optional_index::not_found();

        if (offset < Super::zero_symbols())
        {
            // The symbols is from the zero padding - we don't have to decode
            // that part and can simply drop it
            coefficients = split_upper(coefficients, offset.value,
                                       Super::zero_symbols().value);

            // We have to insert the symbol (without the zero padding)
            pivot = Super::decode_inner_symbol(
                symbol_index{Super::zero_symbols().value}, coefficients,
                symbol);
        }
        else
        {
            // Pass the symbol to the inner decoder
            pivot = Super::decode_inner_symbol(offset, coefficients, symbol);
        }

        if (!pivot.found())
        {
            // If we did not find a pivot - nothing more to do
            return pivot;
        }

        if (Super::is_outer_pivot(pivot.index()))
        {
            // We did find a pivot - but it is already a pivot in the outer
            // code so we have to move it
            pivot = Super::move_outer_symbol(pivot.index());

            if (!pivot.found())
            {
                return pivot;
            }
        }

        if (m_outer_decoding_in_progress)
        {
            // If we have already mapped the outer symbols held by the inner
            // code to the outer decoder we immediately map any outer code
            // symbols
            symbol_type type = Super::map_to_type(pivot.index());

            if (type == symbol_type::outer)
            {
                pivot = Super::map_inner_to_outer_symbol(pivot.index());
            }
        }
        else if (Super::should_map_to_outer())
        {
            Super::log("Starting outer decoding process");
            Super::map_to_outer_symbols();
            m_outer_decoding_in_progress = true;
        }

        if (is_complete())
        {
            Super::log("Finalizing decoding");
            // We have pivot for all the data symbols so we can finalize
            // decoding
            backward_substitution();

            if (Super::has_partial_symbol())
            {
                // If we have a partial symbol we need to restor it. The partial
                // symbol is needed when the input data is not a multiplum of
                // the symbol size. In that case the last symbol will be an
                // internal to the library symbol - restoring it simply means
                // we copy the internal symbol memory to the users' buffer.
                Super::restore_partial_symbol();
            }
        }

        return pivot;
    }

    /// Once we have pivots for all data symbols we can run backward
    /// substitution in order to fully decode the data symbols.
    void backward_substitution()
    {
        // In case the last symbol is pivot in the outer code, copy it over.
        if (Super::is_outer_pivot(Super::last_symbol()))
        {
            Super::copy_from_outer_symbol(
                Super::last_symbol(), Super::symbol_data(Super::last_symbol()));
        }

        // We have to loop from the back to the front - the reason we do it this
        // way is that by working from the back to the front we know that any
        // pivot data symbols "mixed" with the current symbol must be fully
        // decoded and can therefore be trivially subtracted.
        //
        // We skip the first symbols which are zero symbols.
        // Also we skip the last symbol since that will already be fully decoded
        // at this point
        detail::symbol_range pivots = to_symbol_range(
            symbol_index{Super::zero_symbols().value}, Super::last_symbol());

        for (symbol_index i : reverse(pivots))
        {
            backward_substitution(i);
        }
    }

    /// Run backward substitution for a specific symbol
    void backward_substitution(symbol_index pivot_index)
    {
        assert(Super::map_to_type(pivot_index) != symbol_type::zero);
        if (!is_pivot(pivot_index))
        {
            return;
        }

        if (Super::is_inner_pivot(pivot_index))
        {
            backward_substitution_inner(pivot_index);
        }
        else
        {
            backward_substitution_outer(pivot_index);

            // If the symbol was in the outer code we need to copy it into
            // place
            Super::copy_from_outer_symbol(pivot_index,
                                          Super::symbol_data(pivot_index));
        }
    }

    /// Run backward substition on an inner code symbol
    void backward_substitution_inner(symbol_index pivot_index)
    {
        symbol_index offset = pivot_index + symbol_count{1};
        inner_coefficients_type coefficients =
            Super::inner_coefficients_data(pivot_index);
        uint8_t* symbol = Super::symbol_data(pivot_index);

        subtract_coefficients(symbol, offset, coefficients);
    }

    /// Run backward substition on an outer code symbol
    void backward_substitution_outer(symbol_index pivot_index)
    {
        symbol_index offset = pivot_index + symbol_count{1};
        inner_coefficients_type* coefficients =
            Super::outer_coefficients_data(pivot_index);
        uint8_t* symbol = Super::outer_symbol_data(pivot_index);

        subtract_coefficients(symbol, offset, coefficients);
    }

    /// Takes a segmented coefficient vector from the outer code and iterates
    /// over each segment to eliminate the symbols
    void subtract_coefficients(uint8_t* symbol, symbol_index offset,
                               inner_coefficients_type* coefficients)
    {
        // Loop though all the segments of the outer encoding vector and
        // subtract mixed symbols
        for (std::size_t i = 0; i < Super::outer_segments(); ++i)
        {
            subtract_coefficients(symbol, offset, coefficients[i]);
            offset += Super::width();

            // if the offset exceeds the upper bound we break out
            if (offset >= Super::symbol_upper_bound())
            {
                return;
            }
        }
    }

    /// Takes a single "width" segment either from the inner code or part of an
    /// outer code encoding vector.
    /// @param symbol The symbol we are operating on
    /// @param offset The symbol index of the first bit in the encoding vector
    /// @param coefficients The coefficients of the encoding vector
    void subtract_coefficients(uint8_t* symbol, symbol_index offset,
                               inner_coefficients_type coefficients)
    {
        // Make sure we won't visit symbols beyond the upper bound
        coefficients = mask_coefficients(coefficients, offset,
                                         Super::symbol_upper_bound());

        // Loop thorugh all the bits and subtract when we have a non-zero bit
        while (coefficients != 0)
        {
            if ((coefficients & 0b1) == 0)
            {
                // Zero bit
                coefficients = coefficients >> 1U;
                ++offset;
                continue;
            }

            subtract_symbol(symbol, offset);

            coefficients = coefficients >> 1U;
            ++offset;
        }
    }

    /// Subtracts a specific symbol
    void subtract_symbol(uint8_t* symbol, symbol_index offset)
    {
        if (is_pivot(offset))
        {
            // If we have a pivot we can directly subtract it
            subtract_pivot(symbol, offset);
        }
        else
        {
            // Must be an outer symbol - since we do not start backward
            // substitution before we have all data symbols as pivot in either
            // the inner or outer code.
            assert(Super::map_to_type(offset) == symbol_type::outer);

            // Since we do not have this outer symbol as pivot we cannot
            // easily subtract it - we therefore have to recursively
            // eliminate it. This could be an expensive operation depending
            // on how many times it happens.
            Super::log("Outer symbol ", offset.value,
                       " not pivot, eliminate using recursion.");

            std::vector<inner_coefficients_type> temp_coefficients(
                Super::outer_segments());

            Super::generate_outer_coefficients(offset,
                                               temp_coefficients.data());
            ++offset;

            subtract_coefficients(symbol, offset, temp_coefficients.data());
        }
    }

    /// Subtract a existing pivot. Note that since backward substitution starts
    /// from the last symbol in the decoder and works its way to the front. We
    /// know that all pivots we need to subtract from the current symbol must
    /// already been fully decoded.
    void subtract_pivot(uint8_t* symbol, symbol_index offset)
    {
        assert(is_pivot(offset));

        const uint8_t* pivot_symbol = nullptr;

        if (Super::is_inner_pivot(offset))
        {
            pivot_symbol = Super::symbol_data(offset);
        }
        else
        {
            pivot_symbol = Super::outer_symbol_data(offset);
        }

        Super::vector_subtract_into(symbol, pivot_symbol,
                                    Super::max_symbol_bytes());
    }

    /// Returns the combined rank of the inner code and the outer code.
    ///
    /// Note, pivots in the outer code only refer to data symbols so the
    /// rank should be the data rank of the inner code + the rank of the
    /// outer code
    auto rank() const -> symbol_count
    {
        return Super::inner_data_rank() + Super::outer_rank();
    }

    /// Return true if the decoder is complete
    auto is_complete() const -> bool
    {
        return rank() == Super::data_symbols();
    }

    /// Returns true if the symbol is either a pivot in the inner or outer
    /// code
    auto is_pivot(symbol_index index) const -> bool
    {
        return Super::is_inner_pivot(index) || Super::is_outer_pivot(index);
    }

private:
    /// Track whether we outer decoding
    bool m_outer_decoding_in_progress;
};
}
}
}
}
