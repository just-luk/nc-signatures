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

#include <kodo/detail/to_byte_range.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace fifi::field;

TEST(detail_test_to_byte_range, api)
{
    EXPECT_EQ(byte_range(byte_index{0}, byte_index{2}),
              to_byte_range(binary{},
                            symbol_range{symbol_index{1}, symbol_index{13}}));
    EXPECT_EQ(byte_range(byte_index{0}, byte_index{2}),
              to_byte_range(binary{},
                            symbol_range{symbol_index{1}, symbol_index{16}}));
    EXPECT_EQ(byte_range(byte_index{0}, byte_index{3}),
              to_byte_range(binary{},
                            symbol_range{symbol_index{1}, symbol_index{17}}));
}
