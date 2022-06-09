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

#include "byte_range.hpp"
#include "ceil_division.hpp"
#include "floor_division.hpp"
#include "to_byte_index.hpp"
#include "to_symbol_frame.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

inline auto to_byte_range(fifi::field::binary, const symbol_frame& frame)
    -> byte_range
{

    // The frame lower bound must be zero or a multiple of eight - this is
    // ensured using the bitmask by setting the lower three bits are
    // zero.
    byte_index lower_byte{
        static_cast<std::size_t>(frame.lower_bound().value / 8U)};
    byte_index upper_byte{
        static_cast<std::size_t>(frame.upper_bound().value / 8U)};

    return {lower_byte, upper_byte};
}

inline auto to_byte_range(fifi::field::binary4, const symbol_frame& frame)
    -> byte_range
{
    // The frame lower bound must be zero or a multiple of eight - this is
    // ensured using the bitmask by setting the lower three bits are
    // zero.
    byte_index lower_byte{
        static_cast<std::size_t>(frame.lower_bound().value / 2U)};
    byte_index upper_byte{
        static_cast<std::size_t>(frame.upper_bound().value / 2U)};

    return {lower_byte, upper_byte};
}

inline auto to_byte_range(fifi::field::binary8, const symbol_frame& frame)
    -> byte_range
{
    byte_index lower_byte{static_cast<std::size_t>(frame.lower_bound().value)};
    byte_index upper_byte{static_cast<std::size_t>(frame.upper_bound().value)};

    return {lower_byte, upper_byte};
}

inline auto to_byte_range(fifi::field::binary16, const symbol_frame& frame)
    -> byte_range
{
    byte_index lower_byte{
        static_cast<std::size_t>(frame.lower_bound().value * 2U)};
    byte_index upper_byte{
        static_cast<std::size_t>(frame.upper_bound().value * 2U)};

    return {lower_byte, upper_byte};
}

template <class Field>
inline auto to_byte_range(Field field, const symbol_range& range) -> byte_range
{
    symbol_frame frame = to_symbol_frame(field, range);
    return to_byte_range(field, frame);
}

}
}
}
