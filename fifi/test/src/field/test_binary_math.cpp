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

#include <fifi/field/binary_math.hpp>

#include <gtest/gtest.h>

#include "../helper/random_vector_size.hpp"
#include "../helper/test_add.hpp"
#include "../helper/test_divide.hpp"
#include "../helper/test_invert.hpp"
#include "../helper/test_multiply.hpp"
#include "../helper/test_subtract.hpp"
#include "../helper/test_vector_add_into.hpp"
#include "../helper/test_vector_dot_product.hpp"
#include "../helper/test_vector_multiply_add_into.hpp"
#include "../helper/test_vector_multiply_into.hpp"
#include "../helper/test_vector_multiply_subtract_into.hpp"
#include "../helper/test_vector_subtract_into.hpp"

static constexpr fifi::field::binary_backend backend;

TEST(test_binary_math, add)
{
    fifi::field::binary_math math{&backend};

    helper::test_add(math, 100U);
    EXPECT_EQ(100U,
              math.metric_value(fifi::field_metric::bytes_processed_no_simd));
}

TEST(test_binary_math, subtract)
{
    fifi::field::binary_math math{&backend};

    helper::test_subtract(math, 100U);
    EXPECT_EQ(100U,
              math.metric_value(fifi::field_metric::bytes_processed_no_simd));
}

TEST(test_binary_math, multiply)
{
    fifi::field::binary_math math{&backend};

    helper::test_multiply(math, 100U);
    EXPECT_EQ(100U,
              math.metric_value(fifi::field_metric::bytes_processed_no_simd));
}

TEST(test_binary_math, divide)
{
    fifi::field::binary_math math{&backend};

    helper::test_divide(math, 100U);
    EXPECT_EQ(100U,
              math.metric_value(fifi::field_metric::bytes_processed_no_simd));
}

TEST(test_binary_math, invert)
{
    fifi::field::binary_math math{&backend};

    helper::test_invert(math, 100U);
    EXPECT_EQ(100U,
              math.metric_value(fifi::field_metric::bytes_processed_no_simd));
}

TEST(test_binary_math, vector_add_into)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_add_into(math, size);

    auto expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, vector_subtract_into)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_subtract_into(math, size);

    auto expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, vector_multiply_into)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_multiply_into(math, size,
                                      rand() % fifi::field::binary::order);

    auto expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, vector_multiply_add_into)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_multiply_add_into(math, size,
                                          rand() % fifi::field::binary::order);

    auto expected_size = math.total_processed();

    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, vector_multiply_subtract_into)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_multiply_subtract_into(math, size);

    auto expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, vector_dot_product)
{
    fifi::field::binary_math math{&backend};
    auto size = helper::random_vector_size(1U);
    helper::test_vector_dot_product(math, size, 21);

    auto expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_binary_math, field)
{
    fifi::field::binary_math math{&backend};
    EXPECT_EQ(2U, math.field_info().prime());
}
