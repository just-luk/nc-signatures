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

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
template <class Value, class Increment>
struct range_iterator
{
    /// @param other Another index_range object
    /// @return True of the index_range objects match
    bool operator!=(const range_iterator& other) const
    {
        return m_i != other.m_i;
    }

    /// @return The current value of the index range
    Value operator*() const
    {
        return m_i;
    }

    /// @return Increment the index range and return it.
    range_iterator& operator++()
    {
        if (Increment::value)
        {
            ++m_i;
        }
        else
        {
            --m_i;
        }

        return *this;
    }

    /// The current value of the index range
    Value m_i;
};

}
}
}
