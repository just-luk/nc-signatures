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

#include "../version.hpp"

#include "is_overlapping.hpp"
#include "range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// Returns the intersection of two ranges
template <class V, class T>
inline auto intersection(const range<V, T>& a, const range<V, T>& b)
    -> range<V, T>
{
    assert(is_overlapping(a, b));

    using std::max;
    using std::min;

    auto lower_bound = max(a.lower_bound(), b.lower_bound());
    auto upper_bound = min(a.upper_bound(), b.upper_bound());

    return range<V, T>{lower_bound, upper_bound};
}

}
}
}
