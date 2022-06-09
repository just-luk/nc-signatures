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

#include <cassert>
#include <cstdint>
#include <limits>
#include <ostream>

#include <bourne/json.hpp>

#include "../version.hpp"

#include "symbol_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a byte count
struct byte_count
{
    /// Default constructor
    byte_count()
    {
    }

    /// Create a byte count from a value
    explicit byte_count(std::size_t byte_count) : value(byte_count)
    {
    }

    /// The value
    std::size_t value = 0;
};

/// byte_count operators
inline auto operator+(byte_count lhs, byte_count rhs) -> byte_count
{
    return byte_count{lhs.value + rhs.value};
}

inline auto operator*(byte_count lhs, symbol_count rhs) -> byte_count
{
    return byte_count{static_cast<std::size_t>(lhs.value * rhs.value)};
}

inline auto operator*(symbol_count lhs, byte_count rhs) -> byte_count
{
    return byte_count{static_cast<std::size_t>(lhs.value * rhs.value)};
}

inline auto operator-(byte_count lhs, byte_count rhs) -> byte_count
{
    assert(lhs.value >= rhs.value);
    return byte_count{lhs.value - rhs.value};
}

inline auto operator+=(byte_count& lhs, byte_count rhs) -> byte_count&
{
    lhs.value += rhs.value;
    return lhs;
}

inline auto operator*=(byte_count& lhs, symbol_count rhs) -> byte_count&
{
    lhs = lhs * rhs;
    return lhs;
}

inline auto operator-=(byte_count& lhs, byte_count rhs) -> byte_count&
{
    assert(lhs.value >= rhs.value);
    lhs.value -= rhs.value;
    return lhs;
}

inline auto operator>(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator>=(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator==(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator!=(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator<=(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator<(byte_count lhs, byte_count rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator<<(std::ostream& out, byte_count bytes) -> std::ostream&
{
    return out << bourne::json({"byte_count", bytes.value}).dump();
}

}
}
}
