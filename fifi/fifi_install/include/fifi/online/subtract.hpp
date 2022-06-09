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
/// @copydoc math::subtract()
constexpr inline uint32_t subtract(field::binary, uint32_t a, uint32_t b)
{
    assert(a < 2U);
    assert(b < 2U);
    return a ^ b;
}

/// @copydoc math::subtract()
constexpr inline uint32_t subtract(field::binary4, uint32_t a, uint32_t b)
{
    assert(a < 16U);
    assert(b < 16U);
    return a ^ b;
}
/// @copydoc math::subtract()
constexpr inline uint32_t subtract(field::binary8, uint32_t a, uint32_t b)
{
    assert(a < 256U);
    assert(b < 256U);
    return a ^ b;
}

/// @copydoc math::subtract()
constexpr inline uint32_t subtract(field::binary16, uint32_t a, uint32_t b)
{
    assert(a < 65536U);
    assert(b < 65536U);
    return a ^ b;
}

/// @copydoc math::subtract()
constexpr inline uint32_t subtract(field::prime2325, uint32_t a, uint32_t b)
{
    const uint32_t prime = field::prime2325().prime();
    assert(a < prime);
    assert(b < prime);

    // Silence warnings if compiling without asserts
    (void)prime;

    // If a >= b, then we will not have an
    // underflow
    //
    //     1 if no underflow
    //     0 if underflow
    //
    // This means that (a >= b) - 1 becomes:
    //
    //     0 or 0x00000000 if no underflow
    //     -1 or 0xffffffff if underflow
    //
    // An underflow is equivalent to a mod 2^32. Since
    // we are working 2^32 - 5 we simply subtract 5 to
    // compensate

    // Different from addition: there is no way we can end up
    // above the prime, so we don't have to do anything else

    return (a - b) + (-5 & ((a >= b) - 1));
}

/// @copydoc math::subtract()
/// @param field the selected finite field
constexpr inline uint32_t subtract(fifi::finite_field field, uint32_t a,
                                   uint32_t b)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return subtract(field::binary(), a, b);
    case fifi::finite_field::binary4:
        return subtract(field::binary4(), a, b);
    case fifi::finite_field::binary8:
        return subtract(field::binary8(), a, b);
    case fifi::finite_field::binary16:
        return subtract(field::binary16(), a, b);
    case fifi::finite_field::prime2325:
        return subtract(field::prime2325(), a, b);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

}
}
}
