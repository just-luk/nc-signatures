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
#include <deque>

#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer for tracing symbol storage information.
template <class Super>
struct layer_log_symbol_storage : public Super
{

    using symbol_pointer_type = typename Super::symbol_pointer_type;

    /// Push a symbol to the stream
    void push_symbol(symbol_pointer_type symbol, byte_count symbol_bytes)
    {
        Super::push_symbol(symbol, symbol_bytes);

        if (Super::is_log_enabled())
        {
            do_log("push_symbol");
        }
    }

    /// Pop a symbol from the stream
    auto pop_symbol() -> symbol_pointer_type
    {
        auto symbol_data = Super::pop_symbol();

        if (Super::is_log_enabled())
        {
            do_log("pop_symbol");
        }

        return symbol_data;
    }

private:
    /// Logs the window info
    void do_log(const std::string& action)
    {
        std::stringstream stream;

        stream << "stream_range = " << Super::stream_range() << "\n";
        stream << "stream_frame = " << Super::stream_frame() << "\n";

        Super::log("stream_layer." + action, stream.str());
    }
};
}
}
}
}
