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

#include <fifi/field/binary8_neon.hpp>

#include <cpuid/cpuinfo.hpp>

#include <gtest/gtest.h>

#include <fifi/detail/is_acceleration_enabled.hpp>

#include "../helper/random_vector_size.hpp"
#include "../helper/test_vector_dot_product.hpp"
#include "../helper/test_vector_multiply_add_into.hpp"
#include "../helper/test_vector_multiply_into.hpp"

static constexpr fifi::field::binary8_neon math(0x11D);

TEST(test_binary8_neon, enabled)
{
    cpuid::cpuinfo m_cpuid;
    EXPECT_EQ(m_cpuid.has_neon(), fifi::detail::is_acceleration_enabled(math));
}

TEST(test_binary8_neon, name)
{
    EXPECT_STREQ("binary8_neon", fifi::field::binary8_neon::name);
    EXPECT_STREQ("binary8_neon", math.name) << math.name;
}

TEST(test_binary8_neon, vector_multiply_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_into(math, helper::random_vector_size(16U));
}

TEST(test_binary8_neon, vector_multiply_add_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_multiply_add_into(math,
                                          helper::random_vector_size(16U));
}

TEST(test_binary8_neon, vector_multiply_add_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_multiply_add_into((uint8_t*)0x1, (uint8_t*)0x1,
                                                0x1, 15U));
}

TEST(test_binary8_neon, vector_dot_product)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_dot_product(math, helper::random_vector_size(16U), 21);
}

TEST(test_binary8_neon, vector_dot_product_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    math.vector_dot_product((uint8_t**)0x1, (const uint8_t**)0x1,
                            (const uint8_t**)0x1, 15U, 1, 1);
}
