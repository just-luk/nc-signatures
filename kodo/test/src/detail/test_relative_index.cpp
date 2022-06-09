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

#include <kodo/detail/relative_index.hpp>

#include <kodo/detail/symbol_frame.hpp>
#include <kodo/detail/symbol_range.hpp>
#include <kodo/detail/to_symbol_frame.hpp>
#include <kodo/detail/to_symbol_range.hpp>

#include <fifi/field/binary8.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace fifi::field;

TEST(detail_test_relative_index, api)
{
    EXPECT_EQ(symbol_index{0},
              relative_index(to_symbol_range(2U, 8U), symbol_index{2U}));
    EXPECT_EQ(symbol_index{2},
              relative_index(to_symbol_range(2U, 8U), symbol_index{4U}));

    EXPECT_EQ(
        symbol_index{0},
        relative_index(to_symbol_frame(binary8{}, 2U, 8U), symbol_index{2U}));
    EXPECT_EQ(
        symbol_index{2},
        relative_index(to_symbol_frame(binary8{}, 2U, 8U), symbol_index{4U}));
}
