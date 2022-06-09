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

#include <kodo/detail/bit_index.hpp>
#include <kodo/detail/to_bitmask.hpp>
#include <kodo/detail/to_byte_index.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_bit_index, api)
{
    bit_index bit{4};

    EXPECT_EQ(bit_index(4U), bit);
    EXPECT_EQ(byte_index(0U), to_byte_index(bit));
    EXPECT_EQ(0b00010000, to_bitmask(bit));

    ++bit;

    EXPECT_EQ(bit_index(5U), bit);
    EXPECT_EQ(byte_index(0U), to_byte_index(bit));
    EXPECT_EQ(0b00100000, to_bitmask(bit));

    ++bit;

    EXPECT_EQ(bit_index(6U), bit);
    EXPECT_EQ(byte_index(0U), to_byte_index(bit));
    EXPECT_EQ(0b01000000, to_bitmask(bit));

    ++bit;

    EXPECT_EQ(bit_index(7U), bit);
    EXPECT_EQ(byte_index(0U), to_byte_index(bit));
    EXPECT_EQ(0b10000000, to_bitmask(bit));

    ++bit;

    EXPECT_EQ(bit_index(8U), bit);
    EXPECT_EQ(byte_index(1U), to_byte_index(bit));
    EXPECT_EQ(0b00000001, to_bitmask(bit));

    ++bit;

    EXPECT_EQ(bit_index(9U), bit);
    EXPECT_EQ(byte_index(1U), to_byte_index(bit));
    EXPECT_EQ(0b00000010, to_bitmask(bit));
}
