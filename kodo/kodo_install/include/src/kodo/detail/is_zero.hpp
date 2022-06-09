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
#include <cstring>

#include "../version.hpp"

#include "byte_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// @param data Pointer to memory
/// @param bytes The size of the buffer in bytes
/// @return True if the buffer is all zero otherwise false.
inline auto is_zero(const void* data, byte_count bytes) -> bool
{
    assert(data != nullptr);
    assert(bytes.value > 0U);

    uint64_t value = 0U;

    std::memcpy(&value, data, std::min(bytes.value, sizeof(value)));

    return (value == 0) &&
           !std::memcmp(data, static_cast<const uint8_t*>(data) + 1U,
                        bytes.value - 1U);
}

}
}
}
