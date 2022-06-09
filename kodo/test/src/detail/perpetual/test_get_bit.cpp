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

#include <kodo/detail/perpetual/get_bit.hpp>

#include <gtest/gtest.h>

TEST(detail_perpetual_test_get_bit, api)
{
    std::vector<uint32_t> data(1);
    data[0] = 0;
    EXPECT_FALSE(kodo::detail::perpetual::get_bit(data.data(), 0, 0));
    data[0] = 1;
    EXPECT_TRUE(kodo::detail::perpetual::get_bit(data.data(), 0, 0));
}
