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

#include "../detail/ceil_division.hpp"

#include "../version.hpp"
#include "binary.hpp"
#include "binary16.hpp"
#include "binary4.hpp"
#include "binary8.hpp"
#include "prime2325.hpp"

#include <cassert>
#include <cstdint>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
/// @copydoc math::elements_to_bytes()
inline std::size_t elements_to_bytes(field::binary, std::size_t elements)
{
    return detail::ceil_division(elements, 8);
}

/// @copydoc math::elements_to_bytes()
inline std::size_t elements_to_bytes(field::binary4, std::size_t elements)
{
    return detail::ceil_division(elements, 2);
}

/// @copydoc math::elements_to_bytes()
inline std::size_t elements_to_bytes(field::binary8, std::size_t elements)
{
    return elements;
}

/// @copydoc math::elements_to_bytes()
inline std::size_t elements_to_bytes(field::binary16, std::size_t elements)
{
    return elements * 2U;
}

/// @copydoc math::elements_to_bytes()
inline std::size_t elements_to_bytes(field::prime2325, std::size_t elements)
{
    return elements * 4U;
}

}
}
}
