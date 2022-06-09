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

#include "../version.hpp"

#include "symbol_count.hpp"
#include "symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// Splits the range into and lower and upper part, where the lower part has
/// a maximum number of elements.
///
/// @param range The range to split
/// @param max_count The maximum number of elements in the returned range
inline auto split_lower(symbol_range range, symbol_count max_count)
    -> symbol_range
{
    symbol_index lower = range.lower_bound();
    symbol_index upper = std::min(lower + max_count, range.upper_bound());

    return symbol_range{lower, upper};
}

}
}
}
