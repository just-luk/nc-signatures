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

#include <kodo/detail/layer_mapping.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    auto field_info() const -> fifi::field::binary8
    {
        return fifi::field::binary8{};
    }
};
}
TEST(detail_test_mapping_layer, in_range)
{
    layer_mapping<dummy> l;
    symbol_index a{1};
    symbol_index b{32};

    symbol_range range{a, b};
    EXPECT_TRUE(l.in_range(range, a));
}

TEST(detail_test_mapping_layer, to_symbol_range)
{
    layer_mapping<dummy> l;
    symbol_index a{1};
    symbol_index b{32};

    {
        symbol_range expected{a, b};
        EXPECT_EQ(expected, l.to_symbol_range(a, b));
    }

    {
        symbol_count c{31};
        symbol_range expected{a, b};
        EXPECT_EQ(expected, l.to_symbol_range(a, c));
    }

    {
        symbol_frame f = to_symbol_frame(l.field_info(), a, b);
        symbol_range expected{a, b};
        EXPECT_EQ(expected, l.to_symbol_range(f));
    }
}

TEST(detail_test_mapping_layer, to_symbol_frame)
{
    layer_mapping<dummy> l;
    symbol_index a{1};
    symbol_index b{32};
    symbol_frame expected = to_symbol_frame(l.field_info(), a, b);
    EXPECT_EQ(expected, l.to_symbol_frame(symbol_range{a, b}));
}

// to_coefficients_view(uint8_t*,symbol_range)
// to_coefficients_view(const uint8_t*, const symbol_range&)
// floor_division(std::size_t numerator, std::size_t denominator)
// ceil_division(std::size_t numerator, std::size_t denominator)
// to_byte_count(From from);
