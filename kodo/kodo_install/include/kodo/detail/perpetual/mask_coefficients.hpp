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

#include "../symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// Masks an encoding vector such that invalid bit positions are zeroed. As an
/// example if we had a total of 38 symbols (indexed from 0 to 37) and we had an
/// encoding vector (32 bit integer) where the first bit represented symbol
/// number 10 we would have the following valid bits:
///
///      +-----+------+-----+
///      |bit  |symbol|is   |
///      |index|offset|valid|
///      +-----+------+-----+
///      |  0  |  10  | yes |
///      |  1  |  11  | yes |
///      |  2  |  12  | yes |
///      |  3  |  13  | yes |
///      |  4  |  14  | yes |
///      |  5  |  15  | yes |
///      |  6  |  16  | yes |
///      |  8  |  17  | yes |
///      |  9  |  18  | yes |
///      | 10  |  19  | yes |
///      | 11  |  20  | yes |
///      | 12  |  21  | yes |
///      | 13  |  22  | yes |
///      | 14  |  23  | yes |
///      | 15  |  24  | yes |
///      | 16  |  25  | yes |
///      | 17  |  26  | yes |
///      | 18  |  27  | yes |
///      | 19  |  28  | yes |
///      | 20  |  29  | yes |
///      | 21  |  30  | yes |
///      | 22  |  31  | yes |
///      | 23  |  32  | yes |
///      | 24  |  33  | yes |
///      | 25  |  34  | yes |
///      | 26  |  35  | yes |
///      | 27  |  36  | yes |
///      | 28  |  37  | yes |
///      | 29  |  38  | no  |  <- invalid
///      | 30  |  39  | no  |  <- invalid
///      | 31  |  40  | no  |  <- invalid
///      +-----+------+-----+
///
/// As we see bit at index 29, 30 and 31 in the 32 bit integer do not represent
/// valid symbol indicies.
///
/// We therefore want to zero those bits to avoid visiting them if iterating
/// thought the bits of an encoding vector.
///
template <class T>
inline auto mask_coefficients(T coefficients, symbol_index offset,
                              symbol_index upper_bound) -> T
{
    assert(upper_bound > offset);

    std::size_t shift = upper_bound.value - offset.value;

    // The "left" part of the coefficient vector is store from bit 0 which is
    // the offset until bit position-offset
    T mask = shift >= std::numeric_limits<T>::digits
                 ? std::numeric_limits<T>::max()
                 : (T{1} << (shift)) - 1;

    return coefficients & mask;
}
}
}
}
}
