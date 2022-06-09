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

#include "../version.hpp"

#include <cassert>
#include <cstdint>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
/// A simple function for determining the degree of a binary extension field
/// polynomial representation.
/// @param value A binary extension field element
/// @return the degree of the polynomial representation of the element.
template <class Value>
inline Value find_polynomial_degree(Value value)
{
    Value degree = 0;

    // Add a check for unsigned or arithmetic shifts
    value >>= 1;

    while (value > 0)
    {
        ++degree;
        value >>= 1;
    }

    return degree;
}

}
}
}
