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

#include "../version.hpp"

#include "bit_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a bit index
struct bit_index
{

    /// Default constructor
    bit_index()
    {
    }

    /// Create a bit count from a value
    explicit bit_index(std::size_t bit_index) : value(bit_index)
    {
    }

    /// The value
    std::size_t value = 0;
};

/// bit_index operators
inline auto operator-(bit_index lhs, bit_index rhs) -> bit_count
{
    assert(lhs.value >= rhs.value);
    return bit_count{lhs.value - rhs.value};
}

inline auto operator+(bit_index lhs, bit_count rhs) -> bit_index
{
    return bit_index{lhs.value + rhs.value};
}

inline auto operator--(bit_index& lhs) -> bit_index&
{
    --lhs.value;
    return lhs;
}

inline auto operator++(bit_index& lhs) -> bit_index&
{
    ++lhs.value;
    return lhs;
}

inline auto operator==(bit_index lhs, bit_index rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator!=(bit_index lhs, bit_index rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator<(bit_index lhs, bit_index rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator<(bit_index lhs, bit_count rhs) -> bool
{
    return lhs.value < rhs.value;
}
}
}
}
