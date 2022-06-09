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
#include <ostream>

#include <bourne/json.hpp>

#include "../symbol_count.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// Strong value representing an outer symbol index
struct outer_index
{
    /// Default constructor
    outer_index()
    {
    }

    /// Copy constructors
    outer_index(const outer_index&) = default;
    outer_index(outer_index&&) = default;

    /// Copy assign
    outer_index& operator=(const outer_index&) = default;
    outer_index& operator=(outer_index&&) = default;

    /// Create an outer symbol index from a value
    explicit outer_index(uint64_t index) : value(index)
    {
    }

    /// The value
    uint64_t value = 0;
};

inline auto operator<(outer_index lhs, symbol_count rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator==(outer_index lhs, outer_index rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator!=(outer_index lhs, outer_index rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator-=(outer_index& lhs, symbol_count rhs) -> outer_index&
{
    lhs.value -= rhs.value;
    return lhs;
}

inline auto operator<<(std::ostream& out, outer_index index) -> std::ostream&
{
    return out << bourne::json({"outer_index", index.value}).dump();
}
}
}
}
}
