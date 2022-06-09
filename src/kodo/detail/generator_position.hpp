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

#include <bourne/json.hpp>

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a generator position
struct generator_position
{
    /// Default constructor
    generator_position()
    {
    }

    /// Copy constructors
    generator_position(const generator_position&) = default;
    generator_position(generator_position&&) = default;

    /// Copy assign
    generator_position& operator=(const generator_position&) = default;
    generator_position& operator=(generator_position&&) = default;

    /// Create a position from a value
    explicit generator_position(std::size_t position) : value(position)
    {
    }

    /// The value
    std::size_t value = 0;
};

inline auto operator>=(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator<(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value < rhs.value;
}
inline auto operator>(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator<=(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator!=(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator==(generator_position lhs, generator_position rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator++(generator_position& position) -> generator_position&
{
    ++position.value;
    return position;
}

inline auto operator<<(std::ostream& out, generator_position position)
    -> std::ostream&
{
    return out << bourne::json({"generator_position", position.value}).dump();
}
}
}
}
