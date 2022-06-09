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

#include <fifi/field/bytes_to_elements.hpp>

#include <gtest/gtest.h>

#include <fifi/field/binary.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>
#include <fifi/field/prime2325.hpp>

TEST(test_bytes_to_elements, binary)
{
    fifi::field::binary field;

    EXPECT_EQ(0U, fifi::field::bytes_to_elements(field, 0));
    EXPECT_EQ(16U, fifi::field::bytes_to_elements(field, 2));
    EXPECT_EQ(32U, fifi::field::bytes_to_elements(field, 4));
    EXPECT_EQ(48U, fifi::field::bytes_to_elements(field, 6));
    EXPECT_EQ(112U, fifi::field::bytes_to_elements(field, 14));
}

TEST(test_bytes_to_elements, binary4)
{
    fifi::field::binary4 field;

    EXPECT_EQ(0U, fifi::field::bytes_to_elements(field, 0));
    EXPECT_EQ(2U, fifi::field::bytes_to_elements(field, 1));
    EXPECT_EQ(4U, fifi::field::bytes_to_elements(field, 2));
    EXPECT_EQ(6U, fifi::field::bytes_to_elements(field, 3));
    EXPECT_EQ(8U, fifi::field::bytes_to_elements(field, 4));
    EXPECT_EQ(10U, fifi::field::bytes_to_elements(field, 5));
    EXPECT_EQ(12U, fifi::field::bytes_to_elements(field, 6));
    EXPECT_EQ(14U, fifi::field::bytes_to_elements(field, 7));
}

TEST(test_bytes_to_elements, binary8)
{
    fifi::field::binary8 field;

    EXPECT_EQ(0U, fifi::field::bytes_to_elements(field, 0));
    EXPECT_EQ(2U, fifi::field::bytes_to_elements(field, 2));
    EXPECT_EQ(4U, fifi::field::bytes_to_elements(field, 4));
    EXPECT_EQ(6U, fifi::field::bytes_to_elements(field, 6));
    EXPECT_EQ(8U, fifi::field::bytes_to_elements(field, 8));
    EXPECT_EQ(10U, fifi::field::bytes_to_elements(field, 10));
    EXPECT_EQ(12U, fifi::field::bytes_to_elements(field, 12));
    EXPECT_EQ(14U, fifi::field::bytes_to_elements(field, 14));
}

TEST(test_bytes_to_elements, binary16)
{
    fifi::field::binary16 field;

    EXPECT_EQ(0U, fifi::field::bytes_to_elements(field, 0));
    EXPECT_EQ(2U, fifi::field::bytes_to_elements(field, 4));
    EXPECT_EQ(4U, fifi::field::bytes_to_elements(field, 8));
    EXPECT_EQ(6U, fifi::field::bytes_to_elements(field, 12));
    EXPECT_EQ(8U, fifi::field::bytes_to_elements(field, 16));
    EXPECT_EQ(10U, fifi::field::bytes_to_elements(field, 20));
    EXPECT_EQ(12U, fifi::field::bytes_to_elements(field, 24));
    EXPECT_EQ(14U, fifi::field::bytes_to_elements(field, 28));
}

TEST(test_bytes_to_elements, prime2325)
{
    fifi::field::prime2325 field;

    EXPECT_EQ(0U, fifi::field::bytes_to_elements(field, 0));
    EXPECT_EQ(1U, fifi::field::bytes_to_elements(field, 4));
    EXPECT_EQ(2U, fifi::field::bytes_to_elements(field, 8));
    EXPECT_EQ(3U, fifi::field::bytes_to_elements(field, 12));
    EXPECT_EQ(4U, fifi::field::bytes_to_elements(field, 16));
    EXPECT_EQ(5U, fifi::field::bytes_to_elements(field, 20));
    EXPECT_EQ(6U, fifi::field::bytes_to_elements(field, 24));
    EXPECT_EQ(7U, fifi::field::bytes_to_elements(field, 28));
}
