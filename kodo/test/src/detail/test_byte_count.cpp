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

#include <kodo/detail/byte_count.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_byte_count, api)
{
    byte_count bytes_a;
    EXPECT_EQ(bytes_a.value, 0U);

    bytes_a = byte_count{3U};

    EXPECT_EQ(bytes_a.value, 3U);

    byte_count bytes_b{4U};
    EXPECT_EQ(bytes_b.value, 4U);

    EXPECT_TRUE(byte_count{2U} < byte_count{3U});
    EXPECT_FALSE(byte_count{2U} < byte_count{2U});
    EXPECT_FALSE(byte_count{2U} < byte_count{1U});

    EXPECT_TRUE(byte_count{3U} > byte_count{2U});
    EXPECT_FALSE(byte_count{2U} > byte_count{2U});
    EXPECT_FALSE(byte_count{1U} > byte_count{2U});

    auto sum = byte_count{4} + byte_count{6};
    EXPECT_EQ(sum, byte_count{10});
    sum += byte_count{4};
    EXPECT_EQ(sum, byte_count{14});

    auto product = byte_count{5} * symbol_count{6};
    EXPECT_EQ(product, byte_count{30});
    product *= symbol_count{6};
    EXPECT_EQ(product, byte_count{180});

    auto difference = byte_count{15} - byte_count{6};
    EXPECT_EQ(difference, byte_count{9});
    difference -= byte_count{6};
    EXPECT_EQ(difference, byte_count{3});
    difference -= byte_count{0};
    EXPECT_EQ(difference, byte_count{3});
}
