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

#include <fifi/field/prime2325_sse42.hpp>

#include <gtest/gtest.h>

#include <fifi/detail/is_acceleration_enabled.hpp>

#include "../helper/random_vector_size.hpp"
#include "../helper/test_vector_add_into.hpp"
#include "../helper/test_vector_dot_product.hpp"
#include "../helper/test_vector_multiply_add_into.hpp"
#include "../helper/test_vector_multiply_into.hpp"
#include "../helper/test_vector_multiply_subtract_into.hpp"
#include "../helper/test_vector_subtract_into.hpp"

static constexpr fifi::field::prime2325_sse42 math;

TEST(test_prime2325_sse42, name)
{
    EXPECT_STREQ("prime2325_sse42", fifi::field::prime2325_sse42::name);
    EXPECT_STREQ("prime2325_sse42", math.name) << math.name;
}

TEST(test_prime2325_sse42, vector_add_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_add_into(math, helper::random_vector_size(16U));
}

TEST(test_prime2325_sse42, vector_add_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_add_into((uint8_t*)0x1, (uint8_t*)0x1, 15U));
}

TEST(test_prime2325_sse42, vector_subtract_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_subtract_into(math, helper::random_vector_size(16U));
}

TEST(test_prime2325_sse42, vector_subtract_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_subtract_into((uint8_t*)0x1, (uint8_t*)0x1, 15U));
}

TEST(test_prime2325_sse42, vector_multiply_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_into(math, helper::random_vector_size(16U));
}

TEST(test_prime2325_sse42, vector_multiply_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_into((uint8_t*)0x1, 0x1, 15U));
}

TEST(test_prime2325_sse42, vector_multiply_add_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_add_into(math,
                                          helper::random_vector_size(16U));
}

TEST(test_prime2325_sse42, vector_multiply_add_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_add_into((uint8_t*)0x1, (uint8_t*)0x1,
                                                0x1, 15U));
}

TEST(test_prime2325_sse42, vector_multiply_subtract_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_subtract_into(math,
                                               helper::random_vector_size(16U));
}

TEST(test_prime2325_sse42, vector_multiply_subtract_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_subtract_into((uint8_t*)0x1,
                                                     (uint8_t*)0x1, 0x1, 15U));
}
