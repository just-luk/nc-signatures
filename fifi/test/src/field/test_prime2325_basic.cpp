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

#include <fifi/field/prime2325_basic.hpp>

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

static constexpr fifi::field::prime2325_basic math;

TEST(test_prime2325_basic, name)
{
    EXPECT_STREQ("prime2325_basic", fifi::field::prime2325_basic::name);
    EXPECT_STREQ("prime2325_basic", math.name) << math.name;
}

TEST(test_prime2325_basic, add)
{
    helper::test_add(math);
}

TEST(test_prime2325_basic, subtract)
{
    helper::test_subtract(math);
}

TEST(test_prime2325_basic, multiply)
{
    helper::test_multiply(math);
}

TEST(test_prime2325_basic, divide)
{
    helper::test_divide(math);
}

TEST(test_prime2325_basic, invert)
{
    helper::test_invert(math);
}

TEST(test_prime2325_basic, vector_add_into)
{
    helper::test_vector_add_into(math, helper::random_vector_size(4U));
}

TEST(test_prime2325_basic, vector_subtract_into)
{
    helper::test_vector_subtract_into(math, helper::random_vector_size(4U));
}

TEST(test_prime2325_basic, vector_multiply_into)
{
    helper::test_vector_multiply_into(math, helper::random_vector_size(4U));
}

TEST(test_prime2325_basic, vector_multiply_add_into)
{
    helper::test_vector_multiply_add_into(math, helper::random_vector_size(4U));
}

TEST(test_prime2325_basic, vector_multiply_subtract_into)
{
    helper::test_vector_multiply_subtract_into(math,
                                               helper::random_vector_size(4U));
}

TEST(test_prime2325_basic, vector_dot_product)
{
    helper::test_vector_dot_product(math, helper::random_vector_size(4U), 21);
}
