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

#include <kodo/detail/perpetual/mask_coefficients.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace kodo::detail::perpetual;

TEST(detail_perpetual_test_mask_coefficients, api)
{
    uint32_t value = 0b1011100011U;
    uint32_t left = mask_coefficients(value, symbol_index{4}, symbol_index{6});

    EXPECT_EQ(left, 0b11U);

    left = mask_coefficients(value, symbol_index{4}, symbol_index{9});
    EXPECT_EQ(left, 0b00011U);

    left = mask_coefficients(value, symbol_index{4}, symbol_index{10});
    EXPECT_EQ(left, 0b100011U);

    left = mask_coefficients(value, symbol_index{4}, symbol_index{721});
    EXPECT_EQ(left, value);

    left = mask_coefficients(value, symbol_index{9}, symbol_index{48});
    EXPECT_EQ(left, value);
}
