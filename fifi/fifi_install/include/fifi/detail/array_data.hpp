// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include "../version.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
template <class ValueType, uint32_t Elements>
struct array_data
{
    constexpr array_data() : m_data{0}
    {
    }

    constexpr const ValueType& operator[](std::size_t index) const
    {
        return m_data[index];
    }

    constexpr ValueType& operator[](std::size_t index)
    {
        return m_data[index];
    }

    const ValueType* data() const
    {
        return m_data;
    }

    ValueType* data()
    {
        return m_data;
    }

    alignas(32) ValueType m_data[Elements];
};
}
}
}
