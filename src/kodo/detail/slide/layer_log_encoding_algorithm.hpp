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

#include <cstdint>
#include <sstream>

#include "../byte_count.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer for tracing the encoding algorithm
template <class Super>
struct layer_log_encoding_algorithm : public Super
{
    /// Write a coded symbol
    byte_count encode_symbol(uint8_t* symbol_data, symbol_range window,
                             const uint8_t* coefficients) const
    {

        auto bytes = Super::encode_symbol(symbol_data, window, coefficients);

        if (Super::is_log_enabled())
        {
            std::stringstream stream;
            stream << "symbol data: ";
            print_symbol(stream, symbol_data, bytes);
            Super::log("encoding_algorithm.write_symbol.after ", stream.str());
        }
        return bytes;
    }

    void print_symbol(std::ostream& out, const uint8_t* symbol,
                      byte_count bytes) const
    {
        for (std::size_t i = 0; i < bytes.value; ++i)
        {
            out << std::setw(3) << (uint32_t)symbol[i] << " ";
        }
        out << "\n";
    }
};
}
}
}
}
