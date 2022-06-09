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

#include "../finite_field.hpp"
#include "../version.hpp"

#include <cassert>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
template <class Field>
struct log_table
{
    using value_type = typename Field::value_type;
    using order_type = typename Field::order_type;

    constexpr log_table(Field field) noexcept :
        m_field{field}, m_log{0}, m_antilog{0}
    {
        // Initial value corresponding to x⁰
        value_type value = 1U;

        for (order_type power = 0; power < Field::order; ++power)
        {
            m_log[value] = power;
            m_antilog[power] = value;

            value = (0b1 << (Field::degree - 1)) & value
                        ? (value << 1) ^ m_field.prime()
                        : value << 1;

            value = value & Field::max_value;
        }
    }

    constexpr value_type invert(value_type x) const
    {
        assert(x != 0U);
        value_type x_power = m_log[x];

        value_type inverse_power = Field::max_value - x_power;

        return m_antilog[inverse_power];
    }

    constexpr value_type multiply(value_type x, value_type y) const
    {
        if (x == 0 || y == 0)
        {
            return 0;
        }

        value_type y_power = m_log[y];
        value_type x_power = m_log[x];

        value_type sum = sum_modulo(x_power, y_power);

        return m_antilog[sum];
    }

    constexpr value_type sum_modulo(value_type x_power,
                                    value_type y_power) const
    {

        // This might be a bit tricky, but what we do here is to check
        // for an integer overflow. If an overflow has happened we
        // have to +1 to sum: we may write the sum as (2^m * h) + l,
        // where h is the high-half of the sum and l is the low half.
        // Now we realize that 2^m is congruent to +1 when working mod
        // (2^m - 1) So we may write 1 + l if h not equal zero and all
        // we have to do is to +1 to the sum.

        value_type sum = x_power + y_power;
        sum = sum < x_power ? sum + 1 : sum;

        if (sum > Field::max_value)
        {
            return sum - Field::max_value;
        }
        else
        {
            return sum;
        }
    }

    finite_field field() const
    {
        return Field::field;
    }

    Field field_info() const
    {
        return m_field;
    }

    Field m_field;
    value_type m_log[Field::order];
    value_type m_antilog[Field::order];
};
}
}
}
