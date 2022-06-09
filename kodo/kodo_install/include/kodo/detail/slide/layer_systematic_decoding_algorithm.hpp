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

#include "coefficients_view.hpp"

#include "../in_range.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer containing the systematic decoding algorithm
template <class Super>
struct layer_systematic_decoding_algorithm : public Super
{
    /// Add a source symbol at the decoder.
    ///
    /// @param symbol_data Buffer containing the source symbol's data
    /// @param index The index of the source symbol in the stream
    auto decode_systematic_symbol(uint8_t* symbol_data, byte_count symbol_bytes,
                                  symbol_index index) -> uint8_t*
    {
        assert(symbol_data != nullptr);
        assert(symbol_bytes > byte_count{0U});
        assert(symbol_bytes <= Super::max_symbol_bytes());
        assert(Super::in_stream(index));

        if (Super::is_symbol_decoded(index))
        {
            // Return the pointer for recycling
            return symbol_data;
        }

        if (Super::is_symbol_partially_decoded(index))
        {
            return Super::swap_decode(symbol_data, symbol_bytes, index);
        }
        else
        {
            // The window for a systematic symbol
            symbol_range window{index, Super::stream_upper_bound()};

            // Allocate a vector to store the coefficients - with sufficient
            // room from the start of the coding window to the end of the
            // stream
            auto vector = Super::allocate_coefficients_vector(window);

            // Subtract the new pivot symbol
            Super::set_value(vector, index, 1U);

            // Store the coding vector
            Super::set_coefficients_vector(index, std::move(vector));

            // Set the symbol
            Super::set_symbol_storage(index, symbol_data, symbol_bytes);

            // Mark this symbol as decoded
            Super::set_symbol_decoded(index);

            // Do backwards substitute.
            Super::backward_substitute(index);

            // We claim the pointer until the symbol is pop'ed.
            return nullptr;
        }
    }
};
}
}
}
}
