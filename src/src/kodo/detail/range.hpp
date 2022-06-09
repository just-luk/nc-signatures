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

#include "range_iterator.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Create a range over some value. The Tag parameter is used to
/// allow creation of multiple different ranges e.g.
///
/// using index_range = range<std::size_t, struct _index_range>;
/// using window_range = range<std::size_t, struct _window_range>;
///
/// We follow Dijkstra's advice and start numbering from zero:
/// http://www.cs.utexas.edu/users/EWD/ewd08xx/EWD831.PDF
template <class Value, class Tag>
struct range
{

    using value_type = Value;
    using forward_iterator = range_iterator<Value, std::true_type>;
    using reverse_iterator = range_iterator<Value, std::false_type>;

    range() : m_lower_bound(0), m_upper_bound(0)
    {
    }

    range(value_type lower_bound, value_type upper_bound) :
        m_lower_bound(lower_bound), m_upper_bound(upper_bound)
    {
    }

    /// Default copy construct and copy assign
    range(const range& other) = default;
    range& operator=(const range& other) = default;

    value_type lower_bound() const
    {
        return m_lower_bound;
    }

    value_type upper_bound() const
    {
        return m_upper_bound;
    }

    /// @return A iterator to the beginning of range
    forward_iterator begin() const
    {
        return {m_lower_bound};
    }

    /// @return A iterator to the end of the range
    forward_iterator end() const
    {
        return {m_upper_bound};
    }

    /// @return A iterator to the end of range
    reverse_iterator rbegin() const
    {
        value_type value = m_upper_bound;
        return reverse_iterator{--value};
    }

    /// @return A iterator to the beginning of the range
    reverse_iterator rend() const
    {
        value_type value = m_lower_bound;
        return reverse_iterator{--value};
    }

    /// @param value The value to check
    /// @return True if value is in the range otherwise false
    bool in_range(const value_type& value) const
    {
        return (value >= lower_bound()) && (value < upper_bound());
    }

    /// Operator equal
    /// @param lhs The left hand side
    /// @param rhs The right hand side
    /// @return True if equal otherwise false.
    friend bool operator==(const range& lhs, const range& rhs)
    {
        return (lhs.m_lower_bound == rhs.m_lower_bound) &&
               (lhs.m_upper_bound == rhs.m_upper_bound);
    }

    /// Operator not equal
    /// @param lhs The left hand side
    /// @param rhs The right hand side
    /// @return True if not equal otherwise false.
    friend bool operator!=(const range& lhs, const range& rhs)
    {
        return !(lhs == rhs);
    }

    value_type m_lower_bound;
    value_type m_upper_bound;
};
}
}
}
