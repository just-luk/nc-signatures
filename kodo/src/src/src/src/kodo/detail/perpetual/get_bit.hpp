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

/// Returns the value of a bit in an array of values
///
/// @param data The array of values
/// @param offset The index of the first bit
/// @param index The index of the bit to fetch
///
template <class T>
inline auto get_bit(const T* data, std::size_t offset, std::size_t index)
    -> bool
{
    assert(data != nullptr);
    assert(offset <= index);

    std::size_t relative = index - offset;

    std::size_t array_index = relative / std::numeric_limits<T>::digits;
    std::size_t array_offset = relative % std::numeric_limits<T>::digits;

    return (data[array_index] >> array_offset) & 0x1;
}

}
}
}
}
