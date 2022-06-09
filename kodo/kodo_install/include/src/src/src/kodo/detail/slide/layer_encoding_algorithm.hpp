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
#include <iomanip>

#include "../byte_count.hpp"
#include "../symbol_frame.hpp"
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
/// Basic layer containing the encoding algorithms
template <class Super>
struct layer_encoding_algorithm : public Super
{

    /// Encode a systematic symbol
    byte_count encode_systematic_symbol(uint8_t* symbol_data,
                                        symbol_index index) const
    {
        assert(symbol_data != nullptr);

        assert(Super::in_range(Super::stream_range(), index));

        const uint8_t* symbol = Super::symbol_data(index);
        byte_count bytes = Super::symbol_bytes(index);

        std::memcpy(symbol_data, symbol, bytes.value);
        return bytes;
    }

    /// Encode a symbol
    byte_count encode_symbol(uint8_t* symbol_data, symbol_range window,
                             const uint8_t* coefficients) const
    {
        assert(symbol_data != nullptr);
        assert(coefficients != nullptr);
        symbol_frame frame = Super::to_symbol_frame(window);

        auto view =
            Super::symbol_frame_to_coefficients_view(coefficients, frame);

        assert(Super::get_value(view, window.lower_bound()) == 1U);

        byte_count largest_symbol{0};

        // We now take the rest of the symbols and add them
        for (symbol_index index : window)
        {
            auto coefficient = Super::get_value(view, index);

            if (coefficient == 0)
            {
                continue;
            }

            const uint8_t* from_symbol_data = Super::symbol_data(index);
            byte_count from_symbol_bytes = Super::symbol_bytes(index);

            // The first symbol can be copied directly without zeroing out the
            // data first.
            if (largest_symbol == byte_count{0})
            {
                std::memcpy(symbol_data, from_symbol_data,
                            from_symbol_bytes.value);
                largest_symbol = from_symbol_bytes;
                continue;
            }

            // Make sure the data is zeroed out before using it in the
            // encoding
            if (from_symbol_bytes > largest_symbol)
            {
                std::memset(symbol_data + largest_symbol.value, 0,
                            (from_symbol_bytes - largest_symbol).value);

                largest_symbol = from_symbol_bytes;
            }

            if (coefficient == 1)
            {
                Super::vector_add_into(symbol_data, from_symbol_data,
                                       from_symbol_bytes);
            }
            else
            {
                Super::vector_multiply_add_into(symbol_data, from_symbol_data,
                                                coefficient, from_symbol_bytes);
            }
        }
        return largest_symbol;
    }
};
}
}
}
}
