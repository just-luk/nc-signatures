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

#include <fifi/field/binary.hpp>
#include <fifi/field/prime2325.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>

#include "../version.hpp"

#include "byte_count.hpp"
#include "symbol_frame.hpp"
#include "symbol_index.hpp"
#include "symbol_range.hpp"
#include "to_byte_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

inline auto to_byte_count(const byte_range& range) -> byte_count
{
    return range.upper_bound() - range.lower_bound();
}

template <class Field>
inline auto to_byte_count(Field field, const symbol_range& range) -> byte_count
{
    byte_range bytes = to_byte_range(field, range);
    return to_byte_count(bytes);
}

template <class Field>
inline auto to_byte_count(Field field, const symbol_frame& frame) -> byte_count
{
    byte_range bytes = to_byte_range(field, frame);
    return to_byte_count(bytes);
}
}
}
}
