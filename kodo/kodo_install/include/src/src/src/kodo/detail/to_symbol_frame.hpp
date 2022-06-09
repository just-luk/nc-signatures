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

#include <fifi/field/prime2325.hpp>
#include <fifi/field/binary.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>

#include "../version.hpp"

#include "byte_index.hpp"
#include "byte_range.hpp"

#include "symbol_frame.hpp"
#include "symbol_index.hpp"
#include "symbol_range.hpp"

#include "ceil_division.hpp"
#include "floor_division.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

inline auto to_symbol_frame(fifi::field::binary, symbol_index lower_bound,
                            symbol_index upper_bound) -> symbol_frame
{
    // The frame lower bound must be zero or a multiple of eight - this is
    // ensured using the bitmask by setting the lower three bits are
    // zero.
    std::size_t lower_index = floor_division(lower_bound.value, 8U) * 8U;
    std::size_t upper_index = ceil_division(upper_bound.value, 8U) * 8U;

    return symbol_frame{symbol_index{lower_index}, symbol_index{upper_index}};
}

inline auto to_symbol_frame(fifi::field::binary4, symbol_index lower_bound,
                            symbol_index upper_bound) -> symbol_frame
{
    // The frame lower bound must be zero or a multiple of eight - this is
    // ensured using the bitmask by setting the lower three bits are
    // zero.
    std::size_t lower_index = floor_division(lower_bound.value, 2U) * 2U;
    std::size_t upper_index = ceil_division(upper_bound.value, 2U) * 2U;

    return symbol_frame{symbol_index{lower_index}, symbol_index{upper_index}};
}

inline auto to_symbol_frame(fifi::field::binary8, symbol_index lower_bound,
                            symbol_index upper_bound) -> symbol_frame
{
    return {lower_bound, upper_bound};
}

inline auto to_symbol_frame(fifi::field::binary16, symbol_index lower_bound,
                            symbol_index upper_bound) -> symbol_frame
{
    return {lower_bound, upper_bound};
}
inline auto to_symbol_frame(fifi::field::prime2325, symbol_index lower_bound,
                            symbol_index upper_bound) -> symbol_frame
{
    return {lower_bound, upper_bound};
}

template <class Field>
inline auto to_symbol_frame(Field field, const symbol_range& range)
    -> symbol_frame
{
    return to_symbol_frame(field, range.lower_bound(), range.upper_bound());
}

template <class Field>
inline auto to_symbol_frame(Field field, std::size_t lower_bound,
                            std::size_t upper_bound) -> symbol_frame
{
    assert(lower_bound <= upper_bound);
    return to_symbol_frame(field, symbol_index{lower_bound},
                           symbol_index{upper_bound});
}

inline auto to_symbol_frame(fifi::field::binary, byte_index lower_bound,
                            byte_index upper_bound) -> symbol_frame
{
    symbol_index lower_index{lower_bound.value * 8U};
    symbol_index upper_index{upper_bound.value * 8U};

    return symbol_frame{lower_index, upper_index};
}

inline auto to_symbol_frame(fifi::field::binary4, byte_index lower_bound,
                            byte_index upper_bound) -> symbol_frame
{

    symbol_index lower_index{lower_bound.value * 2U};
    symbol_index upper_index{upper_bound.value * 2U};

    return symbol_frame{lower_index, upper_index};
}

inline auto to_symbol_frame(fifi::field::binary8, byte_index lower_bound,
                            byte_index upper_bound) -> symbol_frame
{
    return {symbol_index{lower_bound.value}, symbol_index{upper_bound.value}};
}

inline auto to_symbol_frame(fifi::field::binary16, byte_index lower_bound,
                            byte_index upper_bound) -> symbol_frame
{

    symbol_index lower_index{ceil_division(lower_bound.value, 2U)};
    symbol_index upper_index{floor_division(upper_bound.value, 2U)};

    return symbol_frame{lower_index, upper_index};
}

inline auto to_symbol_frame(fifi::field::prime2325, byte_index lower_bound,
                            byte_index upper_bound) -> symbol_frame
{

    symbol_index lower_index{ceil_division(lower_bound.value, 4U)};
    symbol_index upper_index{floor_division(upper_bound.value, 4U)};

    return symbol_frame{lower_index, upper_index};
}

template <class Field>
inline auto to_symbol_frame(Field field, const byte_range& range)
    -> symbol_frame
{
    return to_symbol_frame(field, range.lower_bound(), range.upper_bound());
}
}
}
}
