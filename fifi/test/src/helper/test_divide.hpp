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

#include <fifi/online/divide.hpp>

#include <cstdint>

#include <gtest/gtest.h>

#include "random_value.hpp"

namespace helper
{
template <class Math>
void test_divide(const Math& math, uint32_t count = 100)
{
    auto field = math.field();
    // Multiply random values from the field
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t x = random_value(field);
        uint32_t y = random_value(field);

        // Don't divide by 0.
        if (y == 0)
            y += 1;

        uint32_t expected = fifi::online::divide(field, x, y);
        EXPECT_EQ(expected, (uint32_t)math.divide(x, y));
    }
}
}
