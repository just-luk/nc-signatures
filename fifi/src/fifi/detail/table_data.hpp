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
template <class ValueType, std::size_t Rows, std::size_t RowElements>
struct table_data
{
    constexpr table_data() = default;

    constexpr const ValueType* operator[](std::size_t index) const
    {
        return m_data + (index * RowElements);
    }

    constexpr ValueType* operator[](std::size_t index)
    {
        return m_data + (index * RowElements);
    }

    void print_representation(std::ostream& out) const
    {
        std::size_t last_byte = (Rows * RowElements) - 1;

        out << "{";

        for (std::size_t i = 0; i < Rows; ++i)
        {
            const ValueType* row_i = m_data + (i * RowElements);

            for (std::size_t j = 0; j < RowElements; ++j)
            {
                out << (uint64_t)row_i[j];

                std::size_t current_byte = (i * RowElements) + j;

                if (current_byte != last_byte)
                {
                    out << ", ";
                }
            }
            out << "\n";
        }

        out << "}";
    }

    alignas(32) ValueType m_data[Rows * RowElements] = {0};
};
}
}
}
