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

#include <kodo/detail/layer_symbol_bytes.hpp>

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
    stub::function<symbol_count()> symbols;
};
using stack = layer_symbol_bytes<dummy>;
}

TEST(detail_test_layer_symbol_bytes, api)
{
    stack s;

    byte_count symbol_bytes{0x100};
    symbol_count symbols{100};

    s.symbols.set_return(symbols);

    stack::config c;
    c.symbol_bytes = symbol_bytes;
    s.configure(c);

    EXPECT_EQ(symbol_bytes, s.symbol_bytes());
    EXPECT_EQ(symbol_bytes * symbols, s.block_bytes());
}
