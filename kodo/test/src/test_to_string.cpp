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

#include <kodo/to_string.hpp>
#include <kodo/version.hpp>

#include <gtest/gtest.h>

TEST(test_to_string, api)
{
    EXPECT_TRUE(kodo::to_string(kodo::finite_field::binary) == "binary");
    EXPECT_TRUE(kodo::to_string(kodo::finite_field::binary4) == "binary4");
    EXPECT_TRUE(kodo::to_string(kodo::finite_field::binary8) == "binary8");
    EXPECT_TRUE(kodo::to_string(kodo::finite_field::binary16) == "binary16");
}
