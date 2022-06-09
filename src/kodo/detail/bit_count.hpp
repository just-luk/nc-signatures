// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
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
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include <cstdint>
#include <type_traits>

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a bit count
struct bit_count
{
    /// Default constructor
    bit_count()
    {
    }

    /// Create a bit count from a value
    explicit bit_count(std::size_t bit_count) : value(bit_count)
    {
    }

    /// The value
    std::size_t value = 0;
};

/// bit_count operators
inline auto operator<(bit_count lhs, bit_count rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator>(bit_count lhs, bit_count rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator++(bit_count& bits) -> bit_count&
{
    ++bits.value;
    return bits;
}

inline auto operator--(bit_count& bits) -> bit_count&
{
    --bits.value;
    return bits;
}

inline auto operator==(bit_count lhs, bit_count rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator!=(bit_count lhs, bit_count rhs) -> bool
{
    return lhs.value != rhs.value;
}

}
}
}
