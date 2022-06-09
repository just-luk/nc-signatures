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

/// Right shift all the bits in the array.
///
/// Essentially we have an array consisting of a number of, e.g., 32 bit values.
/// We want to be able right shift all of these bits as if we have one bit say
/// 256 bit vector. The shift will be logical meaning that the new top bit
/// introduce will be zero and the bit shifted off the end will be dropped.
///
/// As an example lets say we had four 8 bit values:
///
/// bit
/// index:  7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
///        +---------------+---------------+---------------+---------------+
///        |0 1 1 0 0 1 1 1|0 1 1 0 0 1 1 1|0 1 1 0 0 1 1 1|0 1 1 0 0 1 1 1|
///        +---------------+---------------+---------------+---------------+
/// byte
/// index:        3               2                1               0
///
/// This could be considered one big 4*8 = 32 bit integer. A right shift would
/// shift all the bits one position to the right. Adding a new zero bit at
/// position 7 in byte 3 and dropping bit 0 in byte 0.
///
/// In addition bit 0 in byte 1 would become bit 7 in byte 0 and so forth.
///
/// @param data The array of values
/// @param segments The length of the array
///
template <class T>
inline auto shift_right(T* data, std::size_t segments)
{
    assert(data != nullptr);

    const T shift = std::numeric_limits<T>::digits - 1;

    // Shift the lowest bit away
    data[0] = data[0] >> 1U;

    for (std::size_t i = 1; i < segments; ++i)
    {
        // Check if we need to push a bit to the value precending the
        // i'th value
        T overflow = data[i] & 0b1;
        data[i - 1] = (overflow << shift) | data[i - 1];

        data[i] = data[i] >> 1U;
    }
}
}
}
}
}
