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

#include <gtest/gtest.h>

#include <fifi/online/subtract.hpp>

#include "random_value.hpp"

namespace helper
{
template <class Math>
void test_subtract(const Math& math, uint32_t count = 100)
{
    auto field = math.field();
    // Add random values from the field
    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t x = random_value(field);
        uint32_t y = random_value(field);

        EXPECT_EQ(fifi::online::subtract(field, x, y),
                  (uint32_t)math.subtract(x, y));
    }
}
}
