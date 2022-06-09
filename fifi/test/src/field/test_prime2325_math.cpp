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

#include <fifi/field/prime2325_math.hpp>

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

static constexpr fifi::field::prime2325_backend backend;

TEST(test_prime2325_math, add)
{
    fifi::field::prime2325_math math{&backend};

    helper::test_add(math, 100U);
    EXPECT_EQ(100U, math.total_processed());
}

TEST(test_prime2325_math, subtract)
{
    fifi::field::prime2325_math math{&backend};

    helper::test_subtract(math, 100U);
    EXPECT_EQ(100U, math.total_processed());
}

TEST(test_prime2325_math, multiply)
{
    fifi::field::prime2325_math math{&backend};

    helper::test_multiply(math, 100U);
    EXPECT_EQ(100U, math.total_processed());
}

TEST(test_prime2325_math, divide)
{
    fifi::field::prime2325_math math{&backend};

    helper::test_divide(math, 100U);
    EXPECT_EQ(100U, math.total_processed());
}

TEST(test_prime2325_math, invert)
{
    fifi::field::prime2325_math math{&backend};

    helper::test_invert(math, 100U);
    EXPECT_EQ(100U, math.total_processed());
}

TEST(test_prime2325_math, vector_add_into)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_add_into(math, size);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, vector_subtract_into)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_subtract_into(math, size);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, vector_multiply_into)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_multiply_into(math, size,
                                      rand() % fifi::field::prime2325::order);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, vector_multiply_add_into)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_multiply_add_into(
        math, size, rand() % fifi::field::prime2325::order);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, vector_multiply_subtract_into)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_multiply_subtract_into(math, size);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, vector_dot_product)
{
    fifi::field::prime2325_math math{&backend};
    uint64_t size = helper::random_vector_size(4U);
    helper::test_vector_dot_product(math, size, 21);

    uint64_t expected_size = math.total_processed();
    EXPECT_EQ(expected_size, size);
}

TEST(test_prime2325_math, field)
{
    fifi::field::prime2325_math math{&backend};
    EXPECT_EQ(4294967291U, math.field_info().prime());
}
