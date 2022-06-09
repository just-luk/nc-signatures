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

#include "byte_count.hpp"
#include "symbol_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Strong value representing a symbol index
struct symbol_index
{
    /// Default constructor
    symbol_index()
    {
    }

    /// Copy constructors
    symbol_index(const symbol_index&) = default;
    symbol_index(symbol_index&&) = default;

    /// Copy assign
    symbol_index& operator=(const symbol_index&) = default;
    symbol_index& operator=(symbol_index&&) = default;

    /// Create a symbol index from a value
    explicit symbol_index(uint64_t symbol_index) : value(symbol_index)
    {
    }

    /// The value
    uint64_t value = 0;
};

/// symbol_index operators
inline auto operator-(symbol_index lhs, symbol_index rhs) -> symbol_count
{
    assert(lhs.value >= rhs.value);
    return symbol_count{lhs.value - rhs.value};
}

inline auto operator*(symbol_index offset, byte_count bytes) -> byte_count
{
    return byte_count{static_cast<std::size_t>(offset.value * bytes.value)};
}

inline auto operator-(symbol_index lhs, symbol_count rhs) -> symbol_index
{
    assert(lhs.value >= rhs.value);
    return symbol_index{lhs.value - rhs.value};
}

inline auto operator+(symbol_index lhs, symbol_count rhs) -> symbol_index
{
    return symbol_index{lhs.value + rhs.value};
}

inline auto operator--(symbol_index& symbol) -> symbol_index&
{
    --symbol.value;
    return symbol;
}

inline auto operator++(symbol_index& symbol) -> symbol_index&
{
    ++symbol.value;
    return symbol;
}

inline auto operator+=(symbol_index& lhs, symbol_count rhs) -> symbol_index&
{
    lhs.value += rhs.value;
    return lhs;
}

inline auto operator-=(symbol_index& lhs, symbol_count rhs) -> symbol_index&
{
    lhs.value -= rhs.value;
    return lhs;
}

inline auto operator>=(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator<(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value < rhs.value;
}
inline auto operator>(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator<=(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator!=(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator==(symbol_index lhs, symbol_index rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator<(symbol_index lhs, symbol_count rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator>(symbol_index lhs, symbol_count rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator<=(symbol_index lhs, symbol_count rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator>=(symbol_index lhs, symbol_count rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator<<(std::ostream& out, symbol_index index) -> std::ostream&
{
    return out << bourne::json({"symbol_index", index.value}).dump();
}
}
}
}
