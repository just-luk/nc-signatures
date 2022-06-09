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

#include <kodo/detail/bitset.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_bitset, api)
{
    bitset bs{bit_count{4}};

    EXPECT_EQ(bit_count{4U}, bs.bits());

    for (uint32_t i = 0; i < bs.bits().value; ++i)
    {
        EXPECT_EQ(false, bs.get(bit_index(i)));
    }

    bs.set(bit_index{0});
    bs.set(bit_index{3});

    EXPECT_EQ(bit_index(0U), bs.lsb());
    EXPECT_EQ(bit_index(3U), bs.msb());

    EXPECT_EQ(true, bs.get(bit_index(0)));
    EXPECT_EQ(false, bs.get(bit_index(1)));
    EXPECT_EQ(false, bs.get(bit_index(2)));
    EXPECT_EQ(true, bs.get(bit_index(3)));

    bs.unset(bit_index(3));
    EXPECT_EQ(bit_index(0U), bs.lsb());
    EXPECT_EQ(bit_index(0U), bs.msb());

    bs.resize(bit_count{15});

    EXPECT_EQ(bit_count{15U}, bs.bits());
    EXPECT_EQ(bit_count(0U), bs.count_ones());

    for (uint32_t i = 0; i < bs.bits().value; ++i)
    {
        EXPECT_EQ(false, bs.get(bit_index(i)));
    }

    bs.set(bit_index(3));
    bs.set(bit_index(6));
    bs.set(bit_index(7));
    bs.set(bit_index(9));
    bs.set(bit_index(10));

    EXPECT_EQ(bit_index(3U), bs.lsb());
    EXPECT_EQ(bit_index(10U), bs.msb());
    EXPECT_EQ(bit_count(5U), bs.count_ones());

    bs.unset(bit_index(10));

    EXPECT_EQ(bit_index(3U), bs.lsb());
    EXPECT_EQ(bit_index(9U), bs.msb());

    bs.set(bit_index(10));

    EXPECT_EQ(bit_index(3U), bs.lsb());
    EXPECT_EQ(bit_index(10U), bs.msb());

    bs.unset(bit_index(3));

    EXPECT_EQ(bit_index(6U), bs.lsb());
    EXPECT_EQ(bit_index(10U), bs.msb());
    EXPECT_EQ(bit_count(4U), bs.count_ones());

    EXPECT_EQ(bit_index(0U), bs.lower_bound());
    EXPECT_EQ(bit_index(15U), bs.upper_bound());

    bs.unset(bit_index(6));

    EXPECT_EQ(bit_index(7U), bs.lsb());
    EXPECT_EQ(bit_index(10U), bs.msb());
}
