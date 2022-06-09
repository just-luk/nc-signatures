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

#include "../version.hpp"

#include "symbol_frame.hpp"
#include "symbol_index.hpp"
#include "symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// @param from The symbol frame to convert
/// @return The new symbol range
inline auto to_symbol_range(const symbol_frame& frame) -> symbol_range
{
    return {frame.lower_bound(), frame.upper_bound()};
}

inline auto to_symbol_range(symbol_index lower_bound, symbol_index upper_bound)
    -> symbol_range
{
    return symbol_range{lower_bound, upper_bound};
}

inline auto to_symbol_range(symbol_index upper_bound) -> symbol_range
{
    return symbol_range{symbol_index{0}, upper_bound};
}

inline auto to_symbol_range(symbol_index lower_bound, symbol_count symbols)
    -> symbol_range
{
    symbol_index upper_bound{lower_bound + symbols};
    return to_symbol_range(lower_bound, upper_bound);
}

inline auto to_symbol_range(symbol_count symbols) -> symbol_range
{
    return to_symbol_range(symbol_index{0}, symbols);
}

inline auto to_symbol_range(std::size_t lower_bound, std::size_t upper_bound)
    -> symbol_range
{
    assert(lower_bound <= upper_bound);
    return to_symbol_range(symbol_index{lower_bound},
                           symbol_index{upper_bound});
}
}
}
}
