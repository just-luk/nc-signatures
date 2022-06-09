// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <fifi/field/get_value.hpp>

#include <gtest/gtest.h>

#include <fifi/field/binary.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>
#include <fifi/field/prime2325.hpp>

TEST(test_get_value, binary)
{
    std::vector<uint8_t> data{0b00001111, 0b10101010};
    fifi::field::binary field;

    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 0));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 1));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 2));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 3));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 4));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 5));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 6));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 7));

    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 8));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 9));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 10));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 11));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 12));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 13));
    EXPECT_EQ(0b0, fifi::field::get_value(field, data.data(), 14));
    EXPECT_EQ(0b1, fifi::field::get_value(field, data.data(), 15));
}

TEST(test_get_value, binary4)
{
    std::vector<uint8_t> data{0xff, 0xf0, 0x0f, 0xab};
    fifi::field::binary4 field;

    EXPECT_EQ(0xf, fifi::field::get_value(field, data.data(), 0));
    EXPECT_EQ(0xf, fifi::field::get_value(field, data.data(), 1));
    EXPECT_EQ(0x0, fifi::field::get_value(field, data.data(), 2));
    EXPECT_EQ(0xf, fifi::field::get_value(field, data.data(), 3));
    EXPECT_EQ(0xf, fifi::field::get_value(field, data.data(), 4));
    EXPECT_EQ(0x0, fifi::field::get_value(field, data.data(), 5));
    EXPECT_EQ(0xb, fifi::field::get_value(field, data.data(), 6));
    EXPECT_EQ(0xa, fifi::field::get_value(field, data.data(), 7));
}

TEST(test_get_value, binary8)
{
    std::vector<uint8_t> data{0xff, 0xf0, 0x0f, 0xab};
    fifi::field::binary8 field;

    EXPECT_EQ(0xff, fifi::field::get_value(field, data.data(), 0));
    EXPECT_EQ(0xf0, fifi::field::get_value(field, data.data(), 1));
    EXPECT_EQ(0x0f, fifi::field::get_value(field, data.data(), 2));
    EXPECT_EQ(0xab, fifi::field::get_value(field, data.data(), 3));
}

TEST(test_get_value, binary16)
{
    std::vector<uint16_t> data{0xfff0, 0x0fab};
    fifi::field::binary16 field;

    EXPECT_EQ(0xfff0,
              fifi::field::get_value(field, (const uint8_t*)data.data(), 0));
    EXPECT_EQ(0x0fab,
              fifi::field::get_value(field, (const uint8_t*)data.data(), 1));
}

TEST(test_get_value, prime2325)
{
    std::vector<uint32_t> data{0xffffff00, 0x00ffaabb};
    fifi::field::prime2325 field;

    EXPECT_EQ(0xffffff00U,
              fifi::field::get_value(field, (const uint8_t*)data.data(), 0));
    EXPECT_EQ(0x00ffaabbU,
              fifi::field::get_value(field, (const uint8_t*)data.data(), 1));
}
