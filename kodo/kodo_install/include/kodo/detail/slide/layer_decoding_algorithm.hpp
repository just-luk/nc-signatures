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

#include "../in_range.hpp"
#include "../optional_index.hpp"
#include "../reverse.hpp"
#include "../symbol_index.hpp"
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
/// Layer containing the decoding algorithms
template <class Super>
struct layer_decoding_algorithm : public Super
{
    /// When adding an uncoded symbol with a specific index and the decoder
    /// already contains a coded symbol in that position, this function performs
    /// the proper swap between the two symbols.
    /// This usually only occurs with packet reordering.
    /// @param symbol_data The data of an encoded symbol
    /// @param symbol_bytes The number of bytes contained in the symbol buffer
    /// @param index the index of the symbol
    auto swap_decode(uint8_t* symbol_data, byte_count symbol_bytes,
                     symbol_index index) -> uint8_t*
    {
        assert(symbol_data != nullptr);
        assert(symbol_bytes > byte_count{0U});
        assert(symbol_bytes <= Super::max_symbol_bytes());
        assert(Super::in_stream(index));
        // We already have a partially decoded symbol in this location
        assert(Super::is_symbol_partially_decoded(index));

        // Extract the data from the partially decoded symbol
        uint8_t* symbol_data_i = Super::symbol_data(index);
        byte_count symbol_bytes_i = Super::symbol_bytes(index);
        auto coefficients_i = Super::coefficients_data(index);

        // Make sure the extracted symbol is indeed a symbol with pivot at the
        // given index.
        assert(Super::get_value(coefficients_i, index) == 1U);

        // Mark the symbol as missing
        Super::set_symbol_missing(index);

        // Subtract the incoming symbol
        Super::vector_subtract_into(symbol_data_i, symbol_data, symbol_bytes);
        Super::set_value(coefficients_i, index, 0U);

        // Now feed the decoder the existing symbol as a new coded symbol.
        // We know that it must contain a pivot index larger than the swapped
        // index.
        symbol_range window{index + symbol_count{1},
                            Super::stream_upper_bound()};
        auto coefficients =
            Super::slide_view(window.lower_bound(), coefficients_i);
        uint8_t* recycle_symbol =
            _decode_symbol(symbol_data_i, symbol_bytes_i, window, coefficients);

        // We need to "mark" the coding vector decoded
        std::fill_n(coefficients_i.data(), coefficients_i.bytes().value, 0U);
        Super::set_value(coefficients_i, index, 1U);

        // Set the pointer and size for the symbol
        Super::set_symbol_storage(index, symbol_data, symbol_bytes);

        // Mark this symbol as fully decoded
        Super::set_symbol_decoded(index);

        // Return the pointer to indicate that the symbol is no longer claimed
        // by the decoder
        return recycle_symbol;
    }

    /// @param symbol_data The data of an encoded symbol
    /// @param symbol_bytes The number of bytes contained in the symbol buffer
    /// @param window The range of symbols contained in the coding vector
    /// @param coefficients The coding vector
    auto decode_symbol(uint8_t* symbol_data, byte_count symbol_bytes,
                       symbol_range window, uint8_t* coefficients) -> uint8_t*
    {
        assert(symbol_data != nullptr);
        assert(symbol_bytes > byte_count{0});
        assert(symbol_bytes <= Super::max_symbol_bytes());
        assert(coefficients != nullptr);

        // Zero the remainding content of symbol_data
        std::memset(symbol_data + symbol_bytes.value, 0,
                    (Super::max_symbol_bytes() - symbol_bytes).value);

        auto coefficients_view = Super::symbol_frame_to_coefficients_view(
            coefficients, Super::to_symbol_frame(window));

        return _decode_symbol(symbol_data, symbol_bytes, window,
                              coefficients_view);
    }

private:
    /// @param symbol_data The data of an encoded symbol
    /// @param symbol_bytes The number of bytes contained in the symbol buffer
    /// @param window The range of symbols contained in the coding vector
    /// @param coefficients_view The coding vector
    auto _decode_symbol(uint8_t* symbol_data, byte_count symbol_bytes,
                        symbol_range window,
                        const mutable_coefficients_view& coefficients_view)
        -> uint8_t*
    {
        assert(symbol_data != nullptr);
        assert(symbol_bytes > byte_count{0});
        assert(symbol_bytes <= Super::max_symbol_bytes());
        assert(coefficients_view.data() != nullptr);
        assert(Super::is_subrange(Super::stream_range(), window));

        // Allocate a vector to store the coefficients - with sufficient
        // room from the start of the coding window to the end of the stream
        auto coefficients = Super::allocate_coefficients_vector(window);

        // Copy the coefficients to the vector
        coefficients.set(coefficients_view);

        auto result = Super::forward_substitute(symbol_data, symbol_bytes,
                                                coefficients.to_mutable_view());
        auto pivot = result.pivot;
        symbol_bytes = result.symbol_bytes;

        if (!pivot.found())
            return symbol_data;

        assert(Super::is_symbol_missing(pivot.index()));

        // Set the pointer and size for the symbol
        Super::set_symbol_storage(pivot.index(), symbol_data, symbol_bytes);
        // Store the coding vector
        Super::set_coefficients_vector(pivot.index(), std::move(coefficients));

        if (!Super::is_binary())
        {
            // Normalize symbol and vector
            Super::normalize(pivot.index());
        }

        if (Super::is_coefficients_decoded(pivot.index()))
        {
            // Mark this symbol as decoded
            Super::set_symbol_decoded(pivot.index());
        }
        else
        {
            // Mark this symbol as partially decoded
            Super::set_symbol_partially_decoded(pivot.index());
        }

        // Backward substitute pivot.
        Super::backward_substitute(pivot.index());

        // Return nullptr to indicate the symbol has been claimed by the decoder
        return nullptr;
    }
};
}
}
}
}
