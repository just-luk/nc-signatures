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

#include <kodo/detail/to_symbol_frame.hpp>
#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace fifi::field;

TEST(detail_test_to_symbol_frame, api)
{
    EXPECT_EQ(to_symbol_frame(binary{}, 0, 16),
              to_symbol_frame(binary{}, to_symbol_range(1, 13)));
    EXPECT_EQ(to_symbol_frame(binary{}, 0, 16),
              to_symbol_frame(binary{}, to_symbol_range(1, 16)));
    EXPECT_EQ(to_symbol_frame(binary{}, 0, 24),
              to_symbol_frame(binary{}, to_symbol_range(1, 17)));
    EXPECT_EQ(to_symbol_frame(binary{}, 8, 24),
              to_symbol_frame(binary{}, to_symbol_range(8, 17)));

    EXPECT_EQ(to_symbol_frame(binary4{}, 0, 14),
              to_symbol_frame(binary4{}, to_symbol_range(1, 13)));
    EXPECT_EQ(to_symbol_frame(binary4{}, 0, 16),
              to_symbol_frame(binary4{}, to_symbol_range(1, 16)));
    EXPECT_EQ(to_symbol_frame(binary4{}, 0, 18),
              to_symbol_frame(binary4{}, to_symbol_range(1, 17)));

    EXPECT_EQ(to_symbol_frame(binary8{}, 1, 13),
              to_symbol_frame(binary8{}, to_symbol_range(1, 13)));
    EXPECT_EQ(to_symbol_frame(binary8{}, 1, 16),
              to_symbol_frame(binary8{}, to_symbol_range(1, 16)));
    EXPECT_EQ(to_symbol_frame(binary8{}, 1, 17),
              to_symbol_frame(binary8{}, to_symbol_range(1, 17)));
}
