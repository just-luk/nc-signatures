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

#include <fifi/field/set_value.hpp>

#include <gtest/gtest.h>

#include <fifi/field/binary.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>
#include <fifi/field/prime2325.hpp>

TEST(test_set_value, binary)
{
    std::vector<uint8_t> data{0x00, 0x00};
    fifi::field::binary field;

    fifi::field::set_value(field, data.data(), 0, 0b1);
    fifi::field::set_value(field, data.data(), 1, 0b1);
    fifi::field::set_value(field, data.data(), 2, 0b1);
    fifi::field::set_value(field, data.data(), 3, 0b1);
    fifi::field::set_value(field, data.data(), 4, 0b0);
    fifi::field::set_value(field, data.data(), 5, 0b0);
    fifi::field::set_value(field, data.data(), 6, 0b0);
    fifi::field::set_value(field, data.data(), 7, 0b0);

    fifi::field::set_value(field, data.data(), 8, 0b0);
    fifi::field::set_value(field, data.data(), 9, 0b1);
    fifi::field::set_value(field, data.data(), 10, 0b0);
    fifi::field::set_value(field, data.data(), 11, 0b1);
    fifi::field::set_value(field, data.data(), 12, 0b0);
    fifi::field::set_value(field, data.data(), 13, 0b1);
    fifi::field::set_value(field, data.data(), 14, 0b0);
    fifi::field::set_value(field, data.data(), 15, 0b1);

    std::vector<uint8_t> expected{0b00001111, 0b10101010};

    EXPECT_EQ(expected, data);
}

TEST(test_set_value, binary4)
{
    std::vector<uint8_t> actual{0x00, 0x00, 0x00, 0x00};
    fifi::field::binary4 field;

    fifi::field::set_value(field, actual.data(), 0, 0xf);
    fifi::field::set_value(field, actual.data(), 1, 0xf);
    fifi::field::set_value(field, actual.data(), 2, 0x0);
    fifi::field::set_value(field, actual.data(), 3, 0xf);
    fifi::field::set_value(field, actual.data(), 4, 0xf);
    fifi::field::set_value(field, actual.data(), 5, 0x0);
    fifi::field::set_value(field, actual.data(), 6, 0xb);
    fifi::field::set_value(field, actual.data(), 7, 0xa);

    std::vector<uint8_t> expected{0xff, 0xf0, 0x0f, 0xab};

    EXPECT_EQ(expected, actual);
}

TEST(test_set_value, binary8)
{
    std::vector<uint8_t> actual{0x00, 0x00, 0x00, 0x00};
    fifi::field::binary8 field;

    fifi::field::set_value(field, actual.data(), 0, 0xff);
    fifi::field::set_value(field, actual.data(), 1, 0xf0);
    fifi::field::set_value(field, actual.data(), 2, 0x0f);
    fifi::field::set_value(field, actual.data(), 3, 0xab);

    std::vector<uint8_t> expected{0xff, 0xf0, 0x0f, 0xab};

    EXPECT_EQ(expected, actual);
}

TEST(test_set_value, binary16)
{
    std::vector<uint16_t> actual{0x0000, 0x0000};
    fifi::field::binary16 field;

    fifi::field::set_value(field, (uint8_t*)actual.data(), 0, 0xffab);
    fifi::field::set_value(field, (uint8_t*)actual.data(), 1, 0xabff);

    std::vector<uint16_t> expected{0xffab, 0xabff};

    EXPECT_EQ(expected, actual);
}

TEST(test_set_value, prime2325)
{
    std::vector<uint32_t> actual{0x00000000, 0x00000000};
    fifi::field::prime2325 field;

    fifi::field::set_value(field, (uint8_t*)actual.data(), 0, 0xffffaabb);
    fifi::field::set_value(field, (uint8_t*)actual.data(), 1, 0xaabbffff);

    std::vector<uint32_t> expected{0xffffaabb, 0xaabbffff};

    EXPECT_EQ(expected, actual);
}
