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

#include <kodo/detail/perpetual/shift_right.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace kodo::detail::perpetual;

TEST(detail_perpetual_test_shift_right, api)
{
    {
        std::vector<uint32_t> values{0x00000001, 0x00000001, 0x00000001,
                                     0x00000001};
        shift_right(values.data(), 4U);

        EXPECT_EQ(values[0], 0x80000000U);
        EXPECT_EQ(values[1], 0x80000000U);
        EXPECT_EQ(values[2], 0x80000000U);
        EXPECT_EQ(values[3], 0x00000000U);
    }

    {
        std::vector<uint32_t> values{0b11, 0b11, 0b11, 0b11};
        shift_right(values.data(), 4U);

        EXPECT_EQ(values[0], 0b10000000000000000000000000000001U);
        EXPECT_EQ(values[1], 0b10000000000000000000000000000001U);
        EXPECT_EQ(values[2], 0b10000000000000000000000000000001U);
        EXPECT_EQ(values[3], 0b00000000000000000000000000000001U);
    }
}
