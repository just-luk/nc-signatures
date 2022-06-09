// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
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
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <kodo/detail/perpetual/inner_generator.hpp>

#include <gtest/gtest.h>

TEST(perpetual_test_inner_generator, api)
{
    std::vector<std::size_t> counter(32, 0);

    kodo::detail::perpetual::inner_generator<uint32_t> generator;

    for (std::size_t i = 0; i < 10000; ++i)
    {
        uint32_t value = generator.generate(i);

        for (std::size_t j = 0; j < 32; ++j)
        {
            counter[j] += 0b1 & value;
            value = value >> 1U;
        }
    }

    EXPECT_EQ(counter[0], 10000U);

    for (std::size_t i = 1; i < 32; ++i)
    {
        EXPECT_GT(counter[i], 3000U);
    }
}
