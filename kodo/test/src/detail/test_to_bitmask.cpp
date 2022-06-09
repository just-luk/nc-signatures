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

#include <kodo/detail/to_bitmask.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_to_bitmask, api)
{
    uint8_t bit7 = 0b10000000;
    uint8_t bit6 = 0b01000000;
    uint8_t bit5 = 0b00100000;
    uint8_t bit4 = 0b00010000;
    uint8_t bit3 = 0b00001000;
    uint8_t bit2 = 0b00000100;
    uint8_t bit1 = 0b00000010;
    uint8_t bit0 = 0b00000001;

    EXPECT_EQ(bit0, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit1, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit2, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit3, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit4, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit5, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit6, (bit0 & to_bitmask(bit_index{0})));
    EXPECT_NE(bit7, (bit0 & to_bitmask(bit_index{0})));

    EXPECT_NE(bit0, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_EQ(bit1, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit2, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit3, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit4, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit5, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit6, (bit1 & to_bitmask(bit_index{1})));
    EXPECT_NE(bit7, (bit1 & to_bitmask(bit_index{1})));

    EXPECT_NE(bit0, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit1, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_EQ(bit2, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit3, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit4, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit5, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit6, (bit2 & to_bitmask(bit_index{2})));
    EXPECT_NE(bit7, (bit2 & to_bitmask(bit_index{2})));

    EXPECT_NE(bit0, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit1, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit2, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_EQ(bit3, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit4, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit5, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit6, (bit3 & to_bitmask(bit_index{3})));
    EXPECT_NE(bit7, (bit3 & to_bitmask(bit_index{3})));

    EXPECT_NE(bit0, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit1, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit2, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit3, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_EQ(bit4, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit5, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit6, (bit4 & to_bitmask(bit_index{4})));
    EXPECT_NE(bit7, (bit4 & to_bitmask(bit_index{4})));

    EXPECT_NE(bit0, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit1, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit2, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit3, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit4, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_EQ(bit5, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit6, (bit5 & to_bitmask(bit_index{5})));
    EXPECT_NE(bit7, (bit5 & to_bitmask(bit_index{5})));

    EXPECT_NE(bit0, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit1, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit2, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit3, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit4, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit5, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_EQ(bit6, (bit6 & to_bitmask(bit_index{6})));
    EXPECT_NE(bit7, (bit6 & to_bitmask(bit_index{6})));

    EXPECT_NE(bit0, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit1, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit2, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit3, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit4, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit5, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_NE(bit6, (bit7 & to_bitmask(bit_index{7})));
    EXPECT_EQ(bit7, (bit7 & to_bitmask(bit_index{7})));
}
