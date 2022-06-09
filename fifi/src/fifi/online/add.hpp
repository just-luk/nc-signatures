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
#include "../finite_field.hpp"
#include "../version.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace online
{
/// @copydoc math::add()
constexpr inline uint32_t add(field::binary, uint32_t a, uint32_t b)
{
    assert(a < 2U);
    assert(b < 2U);

    return a ^ b;
}

/// @copydoc math::add()
constexpr inline uint32_t add(field::binary4, uint32_t a, uint32_t b)
{
    assert(a < 16U);
    assert(b < 16U);

    return a ^ b;
}
/// @copydoc math::add()
constexpr inline uint32_t add(field::binary8, uint32_t a, uint32_t b)
{
    assert(a < 256U);
    assert(b < 256U);

    return a ^ b;
}

/// @copydoc math::add()
constexpr inline uint32_t add(field::binary16, uint32_t a, uint32_t b)
{
    assert(a < 65536U);
    assert(b < 65536U);

    return a ^ b;
}

/// @copydoc math::add()
constexpr inline uint32_t add(field::prime2325, uint32_t a, uint32_t b)
{
    const uint32_t prime = field::prime2325().prime();
    assert(a < prime);
    assert(b < prime);

    a += b;

    // If a >= b, then we did not have a 32 bit overflow:
    //
    //     1 if no overflow
    //     0 if overflow
    //
    // This means that (a >= b) - 1 becomes:
    //
    //     0 or 0x00000000 if no overflow
    //     -1 or 0xffffffff if overflow
    //
    // An overflow is equivalent to a mod 2^32. Since
    // we are working 2^32 - 5 we simply add 5 to compensate
    a += (5 & ((a >= b) - 1));

    // Conditional move version
    // a = a >= field_type::prime ?
    //    a - field_type::prime : a;

    // The line below does the following:
    // field_type::prime > a becomes:
    //
    //     1 if prime is larger
    //     0 if prime is smaller or equal
    //
    // If that means the (field_type::prime > a) - 1 becomes:
    //
    //     0 or 0x00000000 if prime is larger
    //     -1 or 0xffffffff if the prime is smaller or equal
    //
    // This mask is then used to determine whether the subtraction
    // with the prime statement is in effect of not
    //
    // Our measurements show that this branch-less version
    // yields a significant performance gain over both the
    // branched and conditional move versions
    a -= (prime & ((prime > a) - 1));

    return a;
}

/// @copydoc math::add()
/// @param field the selected finite field
constexpr inline uint32_t add(fifi::finite_field field, uint32_t a, uint32_t b)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return add(field::binary(), a, b);
    case fifi::finite_field::binary4:
        return add(field::binary4(), a, b);
    case fifi::finite_field::binary8:
        return add(field::binary8(), a, b);
    case fifi::finite_field::binary16:
        return add(field::binary16(), a, b);
    case fifi::finite_field::prime2325:
        return add(field::prime2325(), a, b);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

}
}
}
