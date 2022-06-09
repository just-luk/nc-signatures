// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>

#include "bytes_to_elements.hpp"
#include "prime2325.hpp"
#include "set_value.hpp"

#include "../version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
/// Overload for applying a prefix to a storage object
/// @param data Buffer where the prefix should be applied.
/// @param size The size of the buffer in bytes.
/// @param prefix to apply
inline void prime2325_apply_prefix(uint8_t* data, std::size_t size,
                                   uint32_t prefix)
{
    assert(data != nullptr);
    assert(size > 0);

    // Size must be multiple of 4 bytes due to the field
    // 2^32 - 5
    assert((size % 4) == 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        uint32_t value = 0;
        std::memcpy(&value, data + (i * sizeof(value)), sizeof(value));

        set_value(prime2325(), data, i, value ^ prefix);
    }
}

}
}
}
