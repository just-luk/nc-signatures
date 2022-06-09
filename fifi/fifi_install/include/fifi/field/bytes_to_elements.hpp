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
/// @copydoc math::bytes_to_elements()
constexpr std::size_t bytes_to_elements(field::binary, std::size_t size)
{
    return size * 8;
}

/// @copydoc math::bytes_to_elements()
constexpr std::size_t bytes_to_elements(field::binary4, std::size_t size)
{
    return size * 2;
}

/// @copydoc math::bytes_to_elements()
constexpr std::size_t bytes_to_elements(field::binary8, std::size_t size)
{
    return size;
}

/// @copydoc math::bytes_to_elements()
constexpr std::size_t bytes_to_elements(field::binary16, std::size_t size)
{
    assert((size % field::binary16::granularity) == 0);
    return size / 2;
}

/// @copydoc math::bytes_to_elements()
constexpr std::size_t bytes_to_elements(field::prime2325, std::size_t size)
{
    assert((size % field::prime2325::granularity) == 0);
    return size / 4;
}
}
}
}
