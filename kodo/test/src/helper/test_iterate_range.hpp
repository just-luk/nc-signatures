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

#pragma once

#include <kodo/detail/reverse.hpp>

namespace helper
{
/// Test helper for testing different ranges
template <class Range>
inline void test_iterate_range()
{
    using value_type = typename Range::value_type;

    {
        // Check upper an lower bound
        value_type lower{3};
        value_type upper{14};

        Range range{lower, upper};

        EXPECT_EQ(lower, range.lower_bound());
        EXPECT_EQ(upper, range.upper_bound());
    }
    // Check that the range generates the correct values
    {
        Range range{value_type{0}, value_type{14}};

        // Forward direction
        std::vector<value_type> expected = {
            value_type{0},  value_type{1}, value_type{2},  value_type{3},
            value_type{4},  value_type{5}, value_type{6},  value_type{7},
            value_type{8},  value_type{9}, value_type{10}, value_type{11},
            value_type{12}, value_type{13}};

        std::vector<value_type> actual;

        for (auto i : range)
        {
            actual.push_back(i);
        }

        EXPECT_EQ(expected, actual);
    }

    {
        Range range{value_type{0}, value_type{14}};

        // Reverse direction
        std::vector<value_type> expected = {
            value_type{13}, value_type{12}, value_type{11}, value_type{10},
            value_type{9},  value_type{8},  value_type{7},  value_type{6},
            value_type{5},  value_type{4},  value_type{3},  value_type{2},
            value_type{1},  value_type{0}};

        std::vector<value_type> actual;

        for (auto i : reverse(range))
        {
            actual.push_back(i);
        }

        EXPECT_EQ(expected, actual);
    }

    {
        // Check zero size range
        Range range{value_type{0}, value_type{0}};

        // We expect it is empty
        std::vector<value_type> expected;
        std::vector<value_type> actual;

        for (auto i : range)
        {
            actual.push_back(i);
        }

        EXPECT_EQ(expected, actual);
    }

    {
        // Check zero size range
        Range range{value_type{10}, value_type{10}};

        // We expect it is empty
        std::vector<value_type> expected;
        std::vector<value_type> actual;

        for (auto i : range)
        {
            actual.push_back(i);
        }

        EXPECT_EQ(expected, actual);
    }
}

}