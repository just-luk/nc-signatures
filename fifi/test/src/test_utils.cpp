// Copyright Steinwurf ApS 2019.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/utils.hpp>

#include <gtest/gtest.h>

TEST(test_utils, elements_to_bytes)
{
    {
        SCOPED_TRACE("binary");
        auto field = fifi::finite_field::binary;
        EXPECT_EQ(1U, fifi::elements_to_bytes(field, 1));
        EXPECT_EQ(1U, fifi::elements_to_bytes(field, 7));
        EXPECT_EQ(1U, fifi::elements_to_bytes(field, 8));
        EXPECT_EQ(2U, fifi::elements_to_bytes(field, 9));
    }

    {
        SCOPED_TRACE("binary4");
        auto field = fifi::finite_field::binary4;
        EXPECT_EQ(1U, fifi::elements_to_bytes(field, 1));
        EXPECT_EQ(4U, fifi::elements_to_bytes(field, 7));
        EXPECT_EQ(4U, fifi::elements_to_bytes(field, 8));
        EXPECT_EQ(5U, fifi::elements_to_bytes(field, 9));
    }

    {
        SCOPED_TRACE("binary8");
        auto field = fifi::finite_field::binary8;
        EXPECT_EQ(1U, fifi::elements_to_bytes(field, 1));
        EXPECT_EQ(9U, fifi::elements_to_bytes(field, 9));
    }

    {
        SCOPED_TRACE("binary16");
        auto field = fifi::finite_field::binary16;
        EXPECT_EQ(10U, fifi::elements_to_bytes(field, 5));
        EXPECT_EQ(26U, fifi::elements_to_bytes(field, 13));
    }

    {
        SCOPED_TRACE("prime2325");
        auto field = fifi::finite_field::prime2325;
        EXPECT_EQ(8U, fifi::elements_to_bytes(field, 2));
        EXPECT_EQ(12U, fifi::elements_to_bytes(field, 3));
    }
}

TEST(test_utils, bytes_to_elements)
{
    {
        SCOPED_TRACE("binary");
        auto field = fifi::finite_field::binary;
        EXPECT_EQ(8U, fifi::bytes_to_elements(field, 1));
        EXPECT_EQ(16U, fifi::bytes_to_elements(field, 2));
    }

    {
        SCOPED_TRACE("binary4");
        auto field = fifi::finite_field::binary4;
        EXPECT_EQ(2U, fifi::bytes_to_elements(field, 1));
        EXPECT_EQ(4U, fifi::bytes_to_elements(field, 2));
    }

    {
        SCOPED_TRACE("binary8");
        auto field = fifi::finite_field::binary8;
        EXPECT_EQ(1U, fifi::bytes_to_elements(field, 1));
        EXPECT_EQ(2U, fifi::bytes_to_elements(field, 2));
    }

    {
        SCOPED_TRACE("binary16");
        auto field = fifi::finite_field::binary16;
        EXPECT_EQ(1U, fifi::bytes_to_elements(field, 2));
        EXPECT_EQ(2U, fifi::bytes_to_elements(field, 4));
    }

    {
        SCOPED_TRACE("prime2325");
        auto field = fifi::finite_field::prime2325;
        EXPECT_EQ(1U, fifi::bytes_to_elements(field, 4));
        EXPECT_EQ(2U, fifi::bytes_to_elements(field, 8));
    }
}

TEST(test_utils, set_value_binary)
{
    auto field = fifi::finite_field::binary;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 7, 1);
    EXPECT_EQ(128U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 1);
    EXPECT_EQ(129U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 0);
    fifi::set_value(field, test.data(), 7, 0);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 8, 1);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(1U, test[1]);
}

TEST(test_utils, set_value_binary4)
{
    auto field = fifi::finite_field::binary4;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 0, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 0);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 1, 1);
    EXPECT_EQ(16U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 1);
    EXPECT_EQ(17U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 0);
    fifi::set_value(field, test.data(), 1, 0);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 2, 1);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(1U, test[1]);

    fifi::set_value(field, test.data(), 0, 0);
    fifi::set_value(field, test.data(), 1, 0);
    fifi::set_value(field, test.data(), 2, 0);
    fifi::set_value(field, test.data(), 3, 0);
    EXPECT_EQ(0U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 0, 15);
    fifi::set_value(field, test.data(), 1, 15);
    fifi::set_value(field, test.data(), 2, 1);
    EXPECT_EQ(255U, test[0]);
    EXPECT_EQ(1U, test[1]);
}

TEST(test_utils, set_value_binary8)
{
    auto field = fifi::finite_field::binary8;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 0, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, test.data(), 1, 42);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(42U, test[1]);

    fifi::set_value(field, test.data(), 0, 16);
    fifi::set_value(field, test.data(), 1, 250);
    EXPECT_EQ(16U, test[0]);
    EXPECT_EQ(250U, test[1]);
}

TEST(test_utils, set_value_binary16)
{
    auto field = fifi::finite_field::binary16;
    std::vector<uint16_t> test{0, 0};

    fifi::set_value(field, (uint8_t*)test.data(), 0, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, (uint8_t*)test.data(), 1, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(1U, test[1]);

    fifi::set_value(field, (uint8_t*)test.data(), 0, 1465);
    fifi::set_value(field, (uint8_t*)test.data(), 1, 47821);
    EXPECT_EQ(1465U, test[0]);
    EXPECT_EQ(47821U, test[1]);
}

TEST(test_utils, set_value_prime2325)
{
    auto field = fifi::finite_field::prime2325;
    std::vector<uint32_t> test{0, 0};

    fifi::set_value(field, (uint8_t*)test.data(), 0, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(0U, test[1]);

    fifi::set_value(field, (uint8_t*)test.data(), 1, 1);
    EXPECT_EQ(1U, test[0]);
    EXPECT_EQ(1U, test[1]);

    fifi::set_value(field, (uint8_t*)test.data(), 0, 0xffffaabb);
    fifi::set_value(field, (uint8_t*)test.data(), 1, 0xaabbffff);
    EXPECT_EQ(0xffffaabbU, test[0]);
    EXPECT_EQ(0xaabbffffU, test[1]);
}

// We only test the binary case here since it relies on fifi::set_value
// internally
TEST(test_utils, set_values)
{
    auto field = fifi::finite_field::binary;

    std::vector<uint8_t> test(fifi::elements_to_bytes(field, 8U));

    fifi::set_values(field, test.data(), {0, 1, 0, 1, 0, 1, 0, 1});

    EXPECT_EQ(fifi::get_value(field, test.data(), 0), 0U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 1), 1U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 2), 0U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 3), 1U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 4), 0U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 5), 1U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 6), 0U);
    EXPECT_EQ(fifi::get_value(field, test.data(), 7), 1U);
}

TEST(test_utils, get_value_binary)
{
    auto field = fifi::finite_field::binary;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 7, 1);
    EXPECT_EQ(1U, fifi::get_value(field, test.data(), 7));
}

TEST(test_utils, get_value_binary4)
{
    auto field = fifi::finite_field::binary4;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 0, 15);
    EXPECT_EQ(15U, test[0]);
    EXPECT_EQ(15U, fifi::get_value(field, test.data(), 0));

    fifi::set_value(field, test.data(), 1, 4);
    EXPECT_EQ(79U, test[0]);
    EXPECT_EQ(4U, fifi::get_value(field, test.data(), 1));
}

TEST(test_utils, get_value_binary8)
{
    auto field = fifi::finite_field::binary8;
    std::vector<uint8_t> test{0, 0};

    fifi::set_value(field, test.data(), 0, 43U);
    EXPECT_EQ(43U, fifi::get_value(field, test.data(), 0));

    fifi::set_value(field, test.data(), 1, 183);
    EXPECT_EQ(183U, fifi::get_value(field, test.data(), 1));
}

TEST(test_utils, get_value_binary16)
{
    auto field = fifi::finite_field::binary16;
    std::vector<uint16_t> test{0, 0};

    fifi::set_value(field, (uint8_t*)test.data(), 0, 610);
    EXPECT_EQ(610U, fifi::get_value(field, (const uint8_t*)test.data(), 0));

    fifi::set_value(field, (uint8_t*)test.data(), 1, 1873);
    EXPECT_EQ(1873U, fifi::get_value(field, (const uint8_t*)test.data(), 1));
}

TEST(test_utils, get_value_prime2325)
{
    auto field = fifi::finite_field::prime2325;
    std::vector<uint32_t> data{0xffffff00, 0x00ffaabb};

    EXPECT_EQ(0xffffff00U,
              fifi::get_value(field, (const uint8_t*)data.data(), 0));
    EXPECT_EQ(0x00ffaabbU,
              fifi::get_value(field, (const uint8_t*)data.data(), 1));
}
