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

#include <kodo/detail/perpetual/outer_generator.hpp>

#include <gtest/gtest.h>

TEST(detail_perpetual_test_outer_generator, api)
{
    kodo::detail::perpetual::outer_generator<uint32_t> generator;

    generator.set_seed(0);

    std::vector<uint32_t> actual(10);
    generator.generate(actual.data(), 10);

    std::vector<uint32_t> expected{
        3380910997, 3122575212, 1252433687, 2043854348, 396657142,
        1659011327, 3292616129, 1738300175, 4120455715, 3437980787};

    EXPECT_EQ(actual, expected);
}
