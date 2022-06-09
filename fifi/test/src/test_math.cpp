// // Copyright Steinwurf ApS 2019.
// // Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// // See accompanying file LICENSE.rst or
// // http://www.steinwurf.com/licensing

#include <fifi/math.hpp>

#include <fifi/field/binary16_backend.hpp>
#include <fifi/field/binary4_backend.hpp>
#include <fifi/field/binary8_backend.hpp>

#include <gtest/gtest.h>

#include "helper/random_vector_size.hpp"
#include "helper/test_add.hpp"
#include "helper/test_divide.hpp"
#include "helper/test_invert.hpp"
#include "helper/test_multiply.hpp"
#include "helper/test_subtract.hpp"
#include "helper/test_vector_add_into.hpp"
#include "helper/test_vector_dot_product.hpp"
#include "helper/test_vector_multiply_add_into.hpp"
#include "helper/test_vector_multiply_into.hpp"
#include "helper/test_vector_multiply_subtract_into.hpp"
#include "helper/test_vector_subtract_into.hpp"

#include <bourne/json.hpp>

TEST(test_math, constructor)
{
    {
        SCOPED_TRACE("default binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(0x2U, math.prime());
    }
    {
        SCOPED_TRACE("default binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(0x13U, math.prime());
    }
    {
        SCOPED_TRACE("default binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(0x11DU, math.prime());
    }
    {
        SCOPED_TRACE("default binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(0x1100BU, math.prime());
    }
    {
        SCOPED_TRACE("default prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(4294967291U, math.prime());
    }
    {
        SCOPED_TRACE("custom binary4");
        static fifi::field::binary4_backend custom_binary4(0x14);
        fifi::math math(&custom_binary4);
        EXPECT_EQ(0x14U, math.prime());
    }
    {
        SCOPED_TRACE("custom binary8");
        static fifi::field::binary8_backend custom_binary8(0x11E);
        fifi::math math(&custom_binary8);
        EXPECT_EQ(0x11EU, math.prime());
    }
    {
        static fifi::field::binary16_backend custom_binary16(0x1100C);
        SCOPED_TRACE("custom binary16");
        fifi::math math(&custom_binary16);
        EXPECT_EQ(0x1100CU, math.prime());
    }
}

TEST(test_math, assignment)
{
    fifi::math math_binary{fifi::finite_field::binary};
    fifi::math math_assign;
    math_assign = math_binary;
    EXPECT_EQ(fifi::finite_field::binary, math_assign.field());

    {
        fifi::math math_binary4{fifi::finite_field::binary4};
        math_assign = std::move(math_binary4);
        EXPECT_EQ(fifi::finite_field::binary4, math_assign.field());
    }

    fifi::math math_assign2(math_binary);
    EXPECT_EQ(fifi::finite_field::binary, math_assign2.field());

    fifi::math math_assign3(std::move(math_binary));
    EXPECT_EQ(fifi::finite_field::binary, math_assign3.field());
}

TEST(test_math, add)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_add(math);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_add(math);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_add(math);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_add(math);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_add(math);
    }
}

TEST(test_math, subtract)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_subtract(math);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_subtract(math);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_subtract(math);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_subtract(math);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_subtract(math);
    }
}

TEST(test_math, multiply)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_multiply(math);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_multiply(math);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_multiply(math);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_multiply(math);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_multiply(math);
    }
}

TEST(test_math, divide)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_divide(math);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_divide(math);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_divide(math);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_divide(math);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_divide(math);
    }
}

TEST(test_math, invert)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_invert(math);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_invert(math);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_invert(math);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_invert(math);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_invert(math);
    }
}

TEST(test_math, vector_add_into)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_add_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_add_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_add_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_add_into(math, helper::random_vector_size(2U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_add_into(math, helper::random_vector_size(4U));
    }
}

TEST(test_math, vector_subtract_into)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_subtract_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_subtract_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_subtract_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_subtract_into(math, helper::random_vector_size(2U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_subtract_into(math, helper::random_vector_size(4U));
    }
}

TEST(test_math, vector_multiply_into)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_multiply_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_multiply_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_multiply_into(math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_multiply_into(math, helper::random_vector_size(2U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_multiply_into(math, helper::random_vector_size(4U));
    }
}

TEST(test_math, vector_multiply_add_into)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_multiply_add_into(math,
                                              helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_multiply_add_into(math,
                                              helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_multiply_add_into(math,
                                              helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_multiply_add_into(math,
                                              helper::random_vector_size(2U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_multiply_add_into(math,
                                              helper::random_vector_size(4U));
    }
}

TEST(test_math, vector_multiply_subtract_into)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_multiply_subtract_into(
            math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_multiply_subtract_into(
            math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_multiply_subtract_into(
            math, helper::random_vector_size(1U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_multiply_subtract_into(
            math, helper::random_vector_size(2U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_multiply_subtract_into(
            math, helper::random_vector_size(4U));
    }
}

TEST(test_math, vector_dot_product)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        helper::test_vector_dot_product(math, helper::random_vector_size(1U),
                                        21);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        helper::test_vector_dot_product(math, helper::random_vector_size(1U),
                                        21);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        helper::test_vector_dot_product(math, helper::random_vector_size(1U),
                                        21);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        helper::test_vector_dot_product(math, helper::random_vector_size(2U),
                                        21);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        helper::test_vector_dot_product(math, helper::random_vector_size(4U),
                                        21);
    }
}

TEST(test_math, granularity)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(1U, math.granularity());
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(1U, math.granularity());
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(1U, math.granularity());
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(2U, math.granularity());
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(4U, math.granularity());
    }
}

TEST(test_math, bytes_to_elements)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(9872U, math.bytes_to_elements(1234U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(2468U, math.bytes_to_elements(1234U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(1234U, math.bytes_to_elements(1234U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(617U, math.bytes_to_elements(1234U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(309U, math.bytes_to_elements(1236U));
    }
}

TEST(test_math, elements_to_bytes)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(6U, math.elements_to_bytes(42U));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(21U, math.elements_to_bytes(42U));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(42U, math.elements_to_bytes(42U));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(84U, math.elements_to_bytes(42U));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(168U, math.elements_to_bytes(42U));
    }
}

TEST(test_math, get_value)
{
    std::vector<uint8_t> all_zero(4, 0);
    std::vector<uint8_t> all_ones(4, 255);

    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(0U, math.get_value(all_zero.data(), 0));
        EXPECT_EQ(1U, math.get_value(all_ones.data(), 0));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(0U, math.get_value(all_zero.data(), 0));
        EXPECT_EQ(15U, math.get_value(all_ones.data(), 0));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(0U, math.get_value(all_zero.data(), 0));
        EXPECT_EQ(255U, math.get_value(all_ones.data(), 0));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(0U, math.get_value(all_zero.data(), 0));
        EXPECT_EQ(65535U, math.get_value(all_ones.data(), 0));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(0U, math.get_value(all_zero.data(), 0));

        // Here all ones does not work since this is not
        // within the field, so instead we use the maximum value
        std::vector<uint8_t> max_value(4);
        uint32_t prime = fifi::field::prime2325().prime();
        math.set_value(max_value.data(), 0, prime - 1);

        EXPECT_EQ(prime - 1, math.get_value(max_value.data(), 0));
    }
}

TEST(test_math, set_value)
{
    std::vector<uint8_t> all_zero(4, 0);
    {
        SCOPED_TRACE("binary");
        std::vector<uint8_t> buffer = all_zero;
        fifi::math math{fifi::finite_field::binary};
        math.set_value(buffer.data(), 0, 1U);
        EXPECT_NE(all_zero, buffer);
    }
    {
        SCOPED_TRACE("binary4");
        std::vector<uint8_t> buffer = all_zero;
        fifi::math math{fifi::finite_field::binary4};
        math.set_value(buffer.data(), 0, 1U);
        EXPECT_NE(all_zero, buffer);
    }
    {
        SCOPED_TRACE("binary8");
        std::vector<uint8_t> buffer = all_zero;
        fifi::math math{fifi::finite_field::binary8};
        math.set_value(buffer.data(), 0, 1U);
        EXPECT_NE(all_zero, buffer);
    }
    {
        SCOPED_TRACE("binary16");
        std::vector<uint8_t> buffer = all_zero;
        fifi::math math{fifi::finite_field::binary16};
        math.set_value(buffer.data(), 0, 1U);
        EXPECT_NE(all_zero, buffer);
    }
    {
        SCOPED_TRACE("prime2325");
        std::vector<uint8_t> buffer = all_zero;
        fifi::math math{fifi::finite_field::prime2325};
        math.set_value(buffer.data(), 0, 1U);
        EXPECT_NE(all_zero, buffer);
    }
}

TEST(test_math, field)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(fifi::finite_field::binary, math.field());
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(fifi::finite_field::binary4, math.field());
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(fifi::finite_field::binary8, math.field());
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(fifi::finite_field::binary16, math.field());
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(fifi::finite_field::prime2325, math.field());
    }
}

TEST(test_math, max_value)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(1U, math.max_value());
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(15U, math.max_value());
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(255U, math.max_value());
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(65535U, math.max_value());
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(4294967290U, math.max_value());
    }
}

TEST(test_math, prime)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(0x2U, math.prime());
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(0x13U, math.prime());
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(0x11DU, math.prime());
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(0x1100BU, math.prime());
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(4294967291U, math.prime());
    }
}

TEST(test_math, metrics_to_json)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        std::error_code error;
        auto json = bourne::json::parse(math.metrics_to_json(), error);
        ASSERT_FALSE((bool)error);
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        std::error_code error;
        auto json = bourne::json::parse(math.metrics_to_json(), error);
        ASSERT_FALSE((bool)error);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        std::error_code error;
        auto json = bourne::json::parse(math.metrics_to_json(), error);
        ASSERT_FALSE((bool)error);
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        std::error_code error;
        auto json = bourne::json::parse(math.metrics_to_json(), error);
        ASSERT_FALSE((bool)error);
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        std::error_code error;
        auto json = bourne::json::parse(math.metrics_to_json(), error);
        ASSERT_FALSE((bool)error);
    }
}

TEST(test_math, processed)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(
            0U, math.metric_value(fifi::field_metric::bytes_processed_no_simd));
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(
            0U, math.metric_value(fifi::field_metric::bytes_processed_no_simd));
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(
            0U, math.metric_value(fifi::field_metric::bytes_processed_no_simd));
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(
            0U, math.metric_value(fifi::field_metric::bytes_processed_no_simd));
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(
            0U, math.metric_value(fifi::field_metric::bytes_processed_no_simd));
    }
}

TEST(test_math, total_processed)
{
    {
        SCOPED_TRACE("binary");
        fifi::math math{fifi::finite_field::binary};
        EXPECT_EQ(0U, math.total_processed());
    }
    {
        SCOPED_TRACE("binary4");
        fifi::math math{fifi::finite_field::binary4};
        EXPECT_EQ(0U, math.total_processed());
    }
    {
        SCOPED_TRACE("binary8");
        fifi::math math{fifi::finite_field::binary8};
        EXPECT_EQ(0U, math.total_processed());
    }
    {
        SCOPED_TRACE("binary16");
        fifi::math math{fifi::finite_field::binary16};
        EXPECT_EQ(0U, math.total_processed());
    }
    {
        SCOPED_TRACE("prime2325");
        fifi::math math{fifi::finite_field::prime2325};
        EXPECT_EQ(0U, math.total_processed());
    }
}
