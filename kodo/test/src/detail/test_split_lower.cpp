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

#include <kodo/detail/split_lower.hpp>

#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_split_lower, api)
{
    {
        symbol_range window = to_symbol_range(0, 6);
        symbol_range expected = to_symbol_range(0, 3);
        EXPECT_EQ(split_lower(window, symbol_count{3}), expected);
    }

    {
        symbol_range window = to_symbol_range(1, 6);
        symbol_range expected = to_symbol_range(1, 4);
        EXPECT_EQ(split_lower(window, symbol_count{3}), expected);
    }

    {
        symbol_range window = to_symbol_range(5, 6);
        symbol_range expected = to_symbol_range(5, 6);
        EXPECT_EQ(split_lower(window, symbol_count{3}), expected);
    }
}
