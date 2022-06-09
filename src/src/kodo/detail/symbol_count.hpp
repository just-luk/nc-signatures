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
/// Strong integer representing a symbol count i.e. a number of symbols
struct symbol_count
{
    /// Default constructor
    symbol_count()
    {
    }

    /// Create a symbol index from a value
    explicit symbol_count(uint64_t symbol_count) : value(symbol_count)
    {
    }

    /// The value
    uint64_t value = 0;
};

inline auto operator+(symbol_count lhs, symbol_count rhs) -> symbol_count
{
    return symbol_count{lhs.value + rhs.value};
}

inline auto operator-(symbol_count lhs, symbol_count rhs) -> symbol_count
{
    assert(lhs.value >= rhs.value);
    return symbol_count{lhs.value - rhs.value};
}

inline auto operator*(symbol_count lhs, symbol_count rhs) -> symbol_count
{
    return symbol_count{lhs.value * rhs.value};
}

inline auto operator<(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator<=(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value <= rhs.value;
}

inline auto operator>(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value > rhs.value;
}

inline auto operator>=(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value >= rhs.value;
}

inline auto operator!=(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value != rhs.value;
}

inline auto operator==(symbol_count lhs, symbol_count rhs) -> bool
{
    return lhs.value == rhs.value;
}

inline auto operator++(symbol_count& symbols) -> symbol_count&
{
    ++symbols.value;
    return symbols;
}

inline auto operator--(symbol_count& symbols) -> symbol_count&
{
    --symbols.value;
    return symbols;
}

inline std::ostream& operator<<(std::ostream& out, symbol_count symbols)
{
    return out << bourne::json({"symbol_count", symbols.value}).dump();
}
}
}
}
