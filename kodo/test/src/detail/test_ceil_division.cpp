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

#include <kodo/detail/ceil_division.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_ceil_division, api)
{
    EXPECT_EQ(0U, ceil_division(0U, 3U));
    EXPECT_EQ(1U, ceil_division(1U, 3U));
    EXPECT_EQ(1U, ceil_division(2U, 3U));
    EXPECT_EQ(1U, ceil_division(3U, 3U));
    EXPECT_EQ(2U, ceil_division(4U, 3U));
}
