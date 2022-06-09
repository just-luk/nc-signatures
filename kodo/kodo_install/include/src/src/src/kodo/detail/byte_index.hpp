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
#include <ostream>
#include <type_traits>

#include <bourne/json.hpp>

#include "../version.hpp"

#include "byte_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a byte index
struct byte_index
{
    /// Default constructor
    byte_index()
    {
    }

    /// Create a byte index from a value
    explicit byte_index(std::size_t byte_index) : value(byte_index)
    {
    }

    /// The value
    std::size_t value = 0;
};

inline auto operator-(byte_index lhs, byte_index rhs) -> byte_count
{
    assert(lhs.value >= rhs.value);
    return byte_count{lhs.value - rhs.value};
}

inline auto operator+(byte_index lhs, byte_count rhs) -> byte_index
{
    return byte_index{lhs.value + rhs.value};
}

inline auto operator<=(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator>=(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator<(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator>(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator==(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator!=(byte_index lhs, byte_index rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator<<(std::ostream& out, byte_index bytes) -> std::ostream&
{
    return out << bourne::json({"byte_index", bytes.value}).dump();
}
}
}
}
