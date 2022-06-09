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
namespace detail
{
template <class Field>
constexpr uint32_t multiply(Field field, uint32_t a, uint32_t b)
{
    uint32_t result = 0;

    if (a != 0 && b != 0)
    {
        uint32_t high_bit = 1 << (Field::degree - 1);
        uint32_t low_bit = 1;

        for (uint32_t i = 0; i < Field::degree; ++i)
        {
            // If b has the low bit set XOR result with a
            result ^= (b & low_bit) ? a : 0;

            bool high_bit_set = high_bit & a;

            // Shift b right to see if we have a non-zero element
            // in the polynomial there.
            b = b >> 1;

            // Shift a left which is equivalent to muliplying with
            // x
            a = a << 1;

            if (high_bit_set)
            {
                // If a became too large (degree equal to our polynomial) we
                // need to reduce a with the prime
                a = a ^ field.prime();
            }

            if (a == 0 || b == 0)
            {
                break;
            }
        }
    }
    return result;
}
}

/// @copydoc math::multiply()
constexpr inline uint32_t multiply(field::binary, uint32_t a, uint32_t b)
{
    assert(a < 2U);
    assert(b < 2U);
    return a & b;
}

/// @copydoc math::multiply()
constexpr inline uint32_t multiply(field::binary4 field, uint32_t a, uint32_t b)
{
    assert(a < 16U);
    assert(b < 16U);
    return detail::multiply(field, a, b);
}
/// @copydoc math::multiply()
constexpr inline uint32_t multiply(field::binary8 field, uint32_t a, uint32_t b)
{
    assert(a < 256U);
    assert(b < 256U);
    return detail::multiply(field, a, b);
}

/// @copydoc math::multiply()
constexpr inline uint32_t multiply(field::binary16 field, uint32_t a,
                                   uint32_t b)
{
    assert(a < 65536U);
    assert(b < 65536U);
    return detail::multiply(field, a, b);
}

/// @copydoc math::multiply()
constexpr inline uint32_t multiply(field::prime2325, uint32_t a, uint32_t b)
{
    const uint32_t prime = field::prime2325().prime();
    assert(a < prime);
    assert(b < prime);

    /// Specialization for the (2^32 - 5) prime field. Multiplication is
    /// based on a clever way of doing modulo. See:
    ///
    ///   S. B. Mohan  and  B. S. Adiga, Electronics  “Fast algorithms for
    ///   implementing rsa public key cryptosystem" Electronics  Letters,
    ///   vol.  21, 1985.
    uint64_t c = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);

    uint32_t l1 = c & 0xffffffff;
    c = c >> 32;
    c = c * 5;

    uint32_t l2 = c & 0xffffffff;
    c = c >> 32;
    c = c * 5;

    c = c + l1 + l2;

    c = c >= prime ? c - prime : c;

    return static_cast<uint32_t>(c);
}

/// @copydoc math::multiply()
/// @param field the selected finite field
constexpr inline uint32_t multiply(fifi::finite_field field, uint32_t a,
                                   uint32_t b)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return multiply(field::binary(), a, b);
    case fifi::finite_field::binary4:
        return multiply(field::binary4(), a, b);
    case fifi::finite_field::binary8:
        return multiply(field::binary8(), a, b);
    case fifi::finite_field::binary16:
        return multiply(field::binary16(), a, b);
    case fifi::finite_field::prime2325:
        return multiply(field::prime2325(), a, b);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

}
}
}
