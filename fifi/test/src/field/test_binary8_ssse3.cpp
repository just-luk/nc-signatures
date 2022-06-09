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

#include <fifi/field/binary8_ssse3.hpp>

#include <cpuid/cpuinfo.hpp>

#include <functional>
#include <gtest/gtest.h>

#include <fifi/detail/is_acceleration_enabled.hpp>

#include "../helper/random_vector_size.hpp"
#include "../helper/test_vector_dot_product.hpp"
#include "../helper/test_vector_multiply_add_into.hpp"
#include "../helper/test_vector_multiply_into.hpp"
#include "../helper/wrap_vector_dot_product.hpp"

static constexpr fifi::field::binary8_ssse3 math(0x11D);

TEST(test_binary8_ssse3, enabled)
{
    cpuid::cpuinfo cpuid;
    EXPECT_EQ(cpuid.has_ssse3(), fifi::detail::is_acceleration_enabled(math));
}

TEST(test_binary8_ssse3, name)
{
    EXPECT_STREQ("binary8_ssse3", fifi::field::binary8_ssse3::name);
    EXPECT_STREQ("binary8_ssse3", math.name) << math.name;
}

TEST(test_binary8_ssse3, vector_multiply_add_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_add_into(math,
                                          helper::random_vector_size(16U));
}

TEST(test_binary8_ssse3, vector_multiply_add_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_add_into((uint8_t*)0x1, (uint8_t*)0x1,
                                                0x1, 15U));
}

TEST(test_binary8_ssse3, vector_multiply_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_into(math, helper::random_vector_size(16U));
}

TEST(test_binary8_ssse3, vector_multiply_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_into((uint8_t*)0x1, 0x1, 15U));
}

TEST(test_binary8_ssse3, vector_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_dot_product(math, helper::random_vector_size(16U), 21);
}

TEST(test_binary8_ssse3, vector_dot_product_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    math.vector_dot_product((uint8_t**)0x1, (const uint8_t**)0x1,
                            (const uint8_t**)0x1, 15U, 1, 1);
}

TEST(test_binary8_ssse3, vector1_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    auto call = [](auto... args) -> uint32_t
    { return math.vector1_dot_product(args...); };

    helper::test_vector_dot_product(helper::wrap_vector_dot_product(call),
                                    math.field(),
                                    helper::random_vector_size(16U), 21);
}

TEST(test_binary8_ssse3, vector2_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    auto call = [](auto... args) -> uint32_t
    { return math.vector2_dot_product(args...); };

    helper::test_vector_dot_product(helper::wrap_vector_dot_product(call),
                                    math.field(),
                                    helper::random_vector_size(16U), 22);
}

TEST(test_binary8_ssse3, vector3_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    auto call = [](auto... args) -> uint32_t
    { return math.vector3_dot_product(args...); };

    helper::test_vector_dot_product(helper::wrap_vector_dot_product(call),
                                    math.field(),
                                    helper::random_vector_size(16U), 27);
}

TEST(test_binary8_ssse3, vector4_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    auto call = [](auto... args) -> uint32_t
    { return math.vector4_dot_product(args...); };

    helper::test_vector_dot_product(helper::wrap_vector_dot_product(call),
                                    math.field(),
                                    helper::random_vector_size(16U), 16);
}
