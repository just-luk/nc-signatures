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

#include <kodo/detail/layer_symbols.hpp>

#include <kodo/detail/byte_count.hpp>
#include <kodo/detail/symbol_count.hpp>
#include <kodo/detail/symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{

struct dummy
{
    struct config
    {
    };
    void configure(const config&)
    {
    }
};
using stack = layer_symbols<dummy>;
}

TEST(detail_test_layer_symbols, api)
{
    stack s;

    symbol_count symbols{100};

    stack::config c;
    c.symbols = symbols;
    s.configure(c);

    EXPECT_EQ(symbols, s.symbols());

    EXPECT_EQ(symbol_index{99}, s.last_symbol());

    symbol_index expected_index{0};
    symbol_index last_symbol{0};
    for (auto index : s.symbol_range())
    {
        last_symbol = index;
        EXPECT_EQ(expected_index, index);
        ++expected_index;
    }
    EXPECT_NE(symbol_index{0}, last_symbol);
    EXPECT_EQ(last_symbol, s.last_symbol());
}
