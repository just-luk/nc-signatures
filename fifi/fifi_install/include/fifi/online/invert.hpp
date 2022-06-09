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

#include "../detail/find_polynomial_degree.hpp"
#include "../field/binary.hpp"
#include "../field/binary16.hpp"
#include "../field/binary4.hpp"
#include "../field/binary8.hpp"
#include "../field/prime2325.hpp"
#include "../finite_field.hpp"
#include "../version.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
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
constexpr inline uint32_t invert(Field field, uint32_t a)
{

    assert(a != 0); // Zero has no inverse
    assert(a < Field::order);

    // If element is 1 the inverse is 1, since we had to
    // 'unwrap' the first iteration (see below), we make an
    // explicit check here.
    if (a == 1U)
        return 1U;

    uint32_t r_large = (uint32_t)field.prime();
    uint32_t r_small = a;

    uint32_t y_large = 0U;
    uint32_t y_small = 1U;

    // Since we cannot use the element data type to represent
    // the field polynomial we would not get a correct degree
    // calculation in the first iteration. Thus we perform the
    // first iteration of the algorithm here with the correct
    // prime degree. In the following iterations of the
    // algorithm the degree of r_large, which initially holds
    // our polynomial will be representable in uint32_t
    int32_t j = Field::degree - fifi::detail::find_polynomial_degree(r_small);

    assert(j > 0);

    r_large ^= (r_small << j);
    y_large ^= (y_small << j);

    // Make sure we are within the max value
    r_large &= Field::order - 1;
    y_large &= Field::order - 1;

    while (r_large != 1)
    {
        assert(r_large > 1);

        j = fifi::detail::find_polynomial_degree(r_large) -
            fifi::detail::find_polynomial_degree(r_small);

        if (j < 0)
        {
            std::swap(r_large, r_small);
            std::swap(y_large, y_small);

            j = std::abs(j);
        }

        r_large ^= (r_small << j);
        y_large ^= (y_small << j);

        // Make sure we are within the max value
        r_large &= Field::max_value;
        y_large &= Field::max_value;
    }

    return y_large;
}
}

/// @copydoc math::invert()
constexpr inline uint32_t invert(field::binary, uint32_t a)
{
    // Only zero and one are valid field elements and zero has
    // no inverse
    assert(a == 1U);

    // Silence warnings if compiling without asserts
    (void)a;

    return 1U;
}

/// @copydoc math::invert()
constexpr inline uint32_t invert(field::binary4 field, uint32_t a)
{
    assert(a < 16U);
    return detail::invert(field, a);
}
/// @copydoc math::invert()
constexpr inline uint32_t invert(field::binary8 field, uint32_t a)
{
    assert(a < 256U);
    return detail::invert(field, a);
}

/// @copydoc math::invert()
constexpr inline uint32_t invert(field::binary16 field, uint32_t a)
{
    assert(a < 65536U);
    return detail::invert(field, a);
}

/// @copydoc math::invert()
constexpr inline uint32_t invert(field::prime2325, uint32_t a)
{
    const uint32_t prime = field::prime2325().prime();
    assert(a > 0);
    assert(a < prime);

    int64_t q = 0;

    int64_t x = 0;
    int64_t x0 = 0;
    int64_t x1 = 1;

    int64_t r = 0;
    int64_t r0 = prime;
    int64_t r1 = a;

    while (r1 != 1)
    {
        // Compute the next remainder
        q = r0 / r1;
        r = r0 - (q * r1);
        x = x0 - (q * x1);

        // Store the results
        r0 = r1;
        r1 = r;
        x0 = x1;
        x1 = x;
    }

    x1 = x1 < 0 ? (x1 + prime) : x1;

    return static_cast<uint32_t>(x1);
}

/// @copydoc math::invert()
/// @param field the selected finite field
constexpr inline uint32_t invert(fifi::finite_field field, uint32_t a)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return invert(field::binary(), a);
    case fifi::finite_field::binary4:
        return invert(field::binary4(), a);
    case fifi::finite_field::binary8:
        return invert(field::binary8(), a);
    case fifi::finite_field::binary16:
        return invert(field::binary16(), a);
    case fifi::finite_field::prime2325:
        return invert(field::prime2325(), a);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

}
}
}
