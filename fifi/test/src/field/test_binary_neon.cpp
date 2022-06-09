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

#include <fifi/field/binary_neon.hpp>

#include <cpuid/cpuinfo.hpp>

#include <gtest/gtest.h>

#include <fifi/detail/is_acceleration_enabled.hpp>

#include "../helper/random_vector_size.hpp"
#include "../helper/test_vector_add_into.hpp"

static constexpr fifi::field::binary_neon math;

TEST(test_binary_neon, enabled)
{
    cpuid::cpuinfo cpuid;
    EXPECT_EQ(cpuid.has_neon(), fifi::detail::is_acceleration_enabled(math));
}

TEST(test_binary_neon, name)
{
    EXPECT_STREQ("binary_neon", fifi::field::binary_neon::name);
    EXPECT_STREQ("binary_neon", math.name) << math.name;
}

TEST(test_binary_neon, vector_add_into)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    helper::test_vector_add_into(math, helper::random_vector_size(16U));
}

TEST(test_binary_neon, vector_add_into_below_granularity)
{
    if (!fifi::detail::is_acceleration_enabled(math))
    {
        return;
    }

    EXPECT_EQ(0U, math.vector_add_into((uint8_t*)0x1, (uint8_t*)0x1, 15U));
}
