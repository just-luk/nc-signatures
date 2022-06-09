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

#include <kodo/slide/rate_controller.hpp>

#include <ctime>
#include <numeric>

#include <gtest/gtest.h>

TEST(slide_test_rate_controller, basic_api)
{
    kodo::slide::rate_controller rate = {6, 4};

    // For every 6 symbols we send 4 are source symbols and 2 are
    // repair.
    EXPECT_FALSE(rate.send_repair());
    rate.advance();

    EXPECT_FALSE(rate.send_repair());
    rate.advance();

    EXPECT_FALSE(rate.send_repair());
    rate.advance();

    EXPECT_FALSE(rate.send_repair());
    rate.advance();

    EXPECT_TRUE(rate.send_repair());
    rate.advance();

    EXPECT_TRUE(rate.send_repair());
    rate.advance();

    // We now start over
    EXPECT_FALSE(rate.send_repair());
    rate.advance();
}
