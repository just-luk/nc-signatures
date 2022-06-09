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

#include "../field/binary.hpp"
#include "../field/binary16.hpp"
#include "../field/binary4.hpp"
#include "../field/binary8.hpp"
#include "../field/prime2325.hpp"
#include "../version.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// @copydoc math::get_value()
inline field::binary::value_type
get_value(field::binary, const uint8_t* elements, std::size_t index)
{
    assert(elements != nullptr);

    std::size_t array_index = index / 8;
    std::size_t offset = index % 8;

    return (elements[array_index] >> offset) & 0x1;
}

/// @copydoc math::get_value()
inline field::binary4::value_type
get_value(field::binary4, const uint8_t* elements, std::size_t index)
{
    assert(elements != nullptr);

    std::size_t array_index = index / 2;

    if (index % 2 == 1)
    {
        // Get upper 4 bits
        return (elements[array_index] & 0xF0) >> 4;
    }
    else
    {
        // Get lower 4 bits
        return elements[array_index] & 0x0F;
    }
}

/// @copydoc math::get_value()
inline field::binary8::value_type
get_value(field::binary8, const uint8_t* elements, std::size_t index)
{
    assert(elements != nullptr);
    return elements[index];
}

/// @copydoc math::get_value()
inline field::binary16::value_type
get_value(field::binary16, const uint8_t* elements, std::size_t index)
{
    assert(elements != nullptr);

    field::binary16::value_type value;
    std::memcpy(&value, elements + (index * sizeof(value)), sizeof(value));

    return value;
}

/// @copydoc math::get_value()
inline field::prime2325::value_type
get_value(field::prime2325, const uint8_t* elements, std::size_t index)
{
    assert(elements != nullptr);

    field::prime2325::value_type value;
    std::memcpy(&value, elements + (index * sizeof(value)), sizeof(value));

    assert(value < field::prime2325::order);

    return value;
}

}
}
}
