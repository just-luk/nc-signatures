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

#include <kodo/detail/reverse.hpp>

#include <kodo/detail/symbol_range.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
namespace
{
symbol_range some_symbol_range()
{
    return symbol_range{symbol_index{0}, symbol_index{10}};
}
}

TEST(detail_test_reverse, api)
{
    std::vector<symbol_index> expected = {
        symbol_index{9}, symbol_index{8}, symbol_index{7}, symbol_index{6},
        symbol_index{5}, symbol_index{4}, symbol_index{3}, symbol_index{2},
        symbol_index{1}, symbol_index{0}};
    std::vector<symbol_index> actual;
    auto max_iterations = expected.size();
    for (symbol_index index : kodo::detail::reverse(some_symbol_range()))
    {
        actual.push_back(symbol_index{index});
        --max_iterations;
        if (max_iterations == 0)
        {
            break;
        }
    }

    EXPECT_EQ(expected, actual);
}
