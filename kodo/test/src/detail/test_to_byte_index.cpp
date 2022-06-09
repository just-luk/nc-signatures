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

#include <kodo/detail/to_byte_index.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;
using namespace fifi::field;

TEST(detail_test_to_byte_index, api)
{

    EXPECT_EQ(byte_index{0U}, to_byte_index(binary{}, symbol_index{1}));
    EXPECT_EQ(byte_index{0U}, to_byte_index(binary{}, symbol_index{7}));
    EXPECT_EQ(byte_index{1U}, to_byte_index(binary{}, symbol_index{8}));

    EXPECT_EQ(byte_index{0U}, to_byte_index(binary4{}, symbol_index{0}));
    EXPECT_EQ(byte_index{0U}, to_byte_index(binary4{}, symbol_index{1}));
    EXPECT_EQ(byte_index{1U}, to_byte_index(binary4{}, symbol_index{2}));
    EXPECT_EQ(byte_index{1U}, to_byte_index(binary4{}, symbol_index{3}));
    EXPECT_EQ(byte_index{2U}, to_byte_index(binary4{}, symbol_index{4}));
    EXPECT_EQ(byte_index{2U}, to_byte_index(binary4{}, symbol_index{5}));
    EXPECT_EQ(byte_index{3U}, to_byte_index(binary4{}, symbol_index{6}));
    EXPECT_EQ(byte_index{3U}, to_byte_index(binary4{}, symbol_index{7}));
    EXPECT_EQ(byte_index{4U}, to_byte_index(binary4{}, symbol_index{8}));

    EXPECT_EQ(byte_index{1U}, to_byte_index(binary8{}, symbol_index{1}));
    EXPECT_EQ(byte_index{7U}, to_byte_index(binary8{}, symbol_index{7}));
    EXPECT_EQ(byte_index{8U}, to_byte_index(binary8{}, symbol_index{8}));

    EXPECT_EQ(byte_index{0U}, to_byte_index(binary16{}, symbol_index{0}));
    EXPECT_EQ(byte_index{2U}, to_byte_index(binary16{}, symbol_index{1}));
    EXPECT_EQ(byte_index{14U}, to_byte_index(binary16{}, symbol_index{7}));
    EXPECT_EQ(byte_index{16U}, to_byte_index(binary16{}, symbol_index{8}));
}
