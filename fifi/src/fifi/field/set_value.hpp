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
/// @copydoc math::set_value()
inline void set_value(field::binary, uint8_t* elements, std::size_t index,
                      field::binary::value_type value)
{
    assert(elements != nullptr);
    assert(value < field::binary::order && "Only 0 and 1 are allowed");

    std::size_t array_index = index / 8;
    std::size_t offset = index % 8;

    field::binary::value_type mask = 1 << offset;

    if (value)
    {
        elements[array_index] |= mask;
    }
    else
    {
        elements[array_index] &= ~mask;
    }
}

/// @copydoc math::set_value()
inline void set_value(field::binary4, uint8_t* elements, std::size_t index,
                      field::binary4::value_type value)
{
    assert(elements != nullptr);
    assert(value < field::binary4::order);

    std::size_t array_index = index / 2;

    if (index % 2 == 1)
    {
        // write upper nibble
        elements[array_index] &= 0x0F;
        elements[array_index] |= (value << 4);
    }
    else
    {
        // write lower nibble
        elements[array_index] &= 0xF0;
        elements[array_index] |= value;
    }
}

/// @copydoc math::set_value()
inline void set_value(field::binary8, uint8_t* elements, std::size_t index,
                      field::binary8::value_type value)
{
    assert(elements != nullptr);

    elements[index] = value;
}

/// @copydoc math::set_value()
inline void set_value(field::binary16, uint8_t* elements, std::size_t index,
                      field::binary16::value_type value)
{
    assert(elements != nullptr);

    std::memcpy(elements + (index * sizeof(field::binary16::value_type)),
                &value, sizeof(field::binary16::value_type));
}

/// @copydoc math::set_value()
inline void set_value(field::prime2325, uint8_t* elements, std::size_t index,
                      field::prime2325::value_type value)
{
    assert(elements != nullptr);
    assert(value < field::prime2325::order);

    std::memcpy(elements + (index * sizeof(field::prime2325::value_type)),
                &value, sizeof(field::prime2325::value_type));
}

}
}
}
