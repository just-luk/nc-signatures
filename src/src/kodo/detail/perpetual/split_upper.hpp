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
#include <limits>

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// Splits the value in two parts and retuns the upper part (most significant
/// bit).
///
/// Example:
///
///      bit index: 7 6 5 4 3 2 1 0
///      bit value: 0 1 0 1 1 0 0 1
///
///      The bit index starts from 0 so the offset is 0 in this case.
///      If we were to split the 8 bit value above at position 5 we would
///      get bits:
///
///      bit index: 7 6 5
///      bit value: 0 1 0
///
/// @param value The value to split
/// @param offset The bit numbering offset
template <class T>
inline auto split_upper(T value, std::size_t offset, std::size_t position) -> T
{
    assert(position >= offset);

    T shift = position - offset;
    return shift >= std::numeric_limits<T>::digits ? 0 : value >> shift;
}
}
}
}
}
