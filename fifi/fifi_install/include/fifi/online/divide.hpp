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
#include "invert.hpp"
#include "multiply.hpp"

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
constexpr uint32_t divide(Field field, uint32_t a, uint32_t b)
{
    return fifi::online::multiply(field, a, fifi::online::invert(field, b));
}
}

/// @copydoc math::divide()
constexpr inline uint32_t divide(field::binary field, uint32_t a, uint32_t b)
{
    assert(a < 2U);
    assert(b < 2U);
    return detail::divide(field, a, b);
}

/// @copydoc math::divide()
constexpr inline uint32_t divide(field::binary4 field, uint32_t a, uint32_t b)
{
    assert(a < 16U);
    assert(b < 16U);
    return detail::divide(field, a, b);
}
/// @copydoc math::divide()
constexpr inline uint32_t divide(field::binary8 field, uint32_t a, uint32_t b)
{
    assert(a < 256U);
    assert(b < 256U);
    return detail::divide(field, a, b);
}

/// @copydoc math::divide()
constexpr inline uint32_t divide(field::binary16 field, uint32_t a, uint32_t b)
{
    assert(a < 65536U);
    assert(b < 65536U);
    return detail::divide(field, a, b);
}

/// @copydoc math::divide()
constexpr inline uint32_t divide(field::prime2325 field, uint32_t a, uint32_t b)
{
    assert(a < field::prime2325().prime());
    assert(b < field::prime2325().prime());
    return detail::divide(field, a, b);
}

/// @copydoc math::divide()
/// @param field the selected finite field
constexpr inline uint32_t divide(fifi::finite_field field, uint32_t a,
                                 uint32_t b)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return divide(field::binary(), a, b);
    case fifi::finite_field::binary4:
        return divide(field::binary4(), a, b);
    case fifi::finite_field::binary8:
        return divide(field::binary8(), a, b);
    case fifi::finite_field::binary16:
        return divide(field::binary16(), a, b);
    case fifi::finite_field::prime2325:
        return divide(field::prime2325(), a, b);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

}
}
}
