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
#include <sstream>

#include "../in_frame.hpp"
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
/// Layer for tracing the decoding algorithm
template <class Super>
struct layer_log_decoding_algorithm : public Super
{
    auto decode_symbol(uint8_t* symbol_data, byte_count symbol_bytes,
                       symbol_range window, uint8_t* coefficients) -> uint8_t*
    {
        if (Super::is_log_enabled())
        {
            std::stringstream stream;
            stream << "window = [" << window.lower_bound().value << ", "
                   << window.upper_bound().value
                   << "[ size = " << symbol_bytes.value << "\n";
            ;
            Super::log("decode_symbol", stream.str());
        }

        uint8_t* recycle_symbol = Super::decode_symbol(
            symbol_data, symbol_bytes, window, coefficients);

        if (Super::is_log_enabled())
        {
            do_log();
        }

        return recycle_symbol;
    }

    auto decode_systematic_symbol(uint8_t* symbol_data, byte_count symbol_bytes,
                                  symbol_index index) -> uint8_t*
    {
        if (Super::is_log_enabled())
        {
            std::stringstream stream;
            stream << "index = " << index.value
                   << " size = " << symbol_bytes.value << "\n";
            Super::log("decode_systematic_symbol", stream.str());
        }
        uint8_t* recycle_symbol =
            Super::decode_systematic_symbol(symbol_data, symbol_bytes, index);

        if (Super::is_log_enabled())
        {
            do_log();
        }

        return recycle_symbol;
    }

private:
    /// Logs the stream info
    void do_log()
    {
        std::stringstream stream;

        symbol_range stream_range = Super::stream_range();
        stream << "stream_range = [" << stream_range.lower_bound().value << ", "
               << stream_range.upper_bound().value << "[\n";

        for (symbol_index i : stream_range)
        {
            stream << i.value << ": ";
            if (Super::has_vector(i))
            {
                auto the_view = Super::coefficients_data(i);
                auto the_frame =
                    Super::coefficients_view_to_symbol_frame(the_view);

                stream << "[ ";
                for (symbol_index j : stream_range)
                {
                    if (in_frame(the_frame, j))
                    {
                        stream << std::setw(3) << Super::get_value(the_view, j)
                               << " ";
                    }
                    else
                    {
                        stream << "  x ";
                    }
                }
                stream << " ] ";
                stream << std::setw(4) << Super::symbol_bytes(i).value
                       << " bytes";
            }
            else
            {
                stream << "none";
            }
            stream << "\n";
        }

        Super::log("decoding_algorithm ", stream.str());
    }
};
}
}
}
}
