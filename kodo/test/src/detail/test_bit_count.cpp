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

#include <kodo/detail/bit_count.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_bit_count, api)
{
    bit_count bits_a;
    EXPECT_EQ(bits_a.value, 0U);

    bits_a = bit_count{3U};

    EXPECT_EQ(bits_a.value, 3U);

    bit_count bits_b{4U};
    EXPECT_EQ(bits_b.value, 4U);

    EXPECT_TRUE(bit_count{2U} < bit_count{3U});
    EXPECT_FALSE(bit_count{2U} < bit_count{2U});
    EXPECT_FALSE(bit_count{2U} < bit_count{1U});

    EXPECT_TRUE(bit_count{3U} > bit_count{2U});
    EXPECT_FALSE(bit_count{2U} > bit_count{2U});
    EXPECT_FALSE(bit_count{1U} > bit_count{2U});
}
