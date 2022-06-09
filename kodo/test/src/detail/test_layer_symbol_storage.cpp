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

#include <kodo/detail/layer_symbol_storage.hpp>

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

    kodo::detail::symbol_range symbol_range() const
    {
        return kodo::detail::symbol_range{symbol_index{0},
                                          symbol_index{symbols().value}};
    }

    stub::function<symbol_count()> symbols;
    stub::function<byte_count()> symbol_bytes;
};
using stack = layer_symbol_storage<uint8_t*, dummy>;
}

TEST(detail_test_layer_symbol_storage, set_symbol_storage)
{
    stack s;

    s.symbol_bytes.set_return(byte_count{0x100});
    s.symbols.set_return(symbol_count{100});

    stack::config c;
    s.configure(c);

    EXPECT_FALSE(s.is_symbol_set(symbol_index{0}));
    EXPECT_FALSE(s.is_symbol_set(symbol_index{1}));
    s.set_symbol_storage((uint8_t*)0x1000, symbol_index{1});
    EXPECT_FALSE(s.is_symbol_set(symbol_index{0}));
    ASSERT_TRUE(s.is_symbol_set(symbol_index{1}));
    EXPECT_EQ((uint8_t*)0x1000, s.symbol_data(symbol_index{1}));
}

TEST(detail_test_layer_symbol_storage, set_symbols_storage)
{
    stack s;

    s.symbol_bytes.set_return(byte_count{0x100});
    s.symbols.set_return(symbol_count{100});

    stack::config c;
    s.configure(c);

    s.set_symbols_storage((uint8_t*)0x1000);

    ASSERT_TRUE(s.is_symbol_set(symbol_index{0}));
    EXPECT_EQ((uint8_t*)0x1000, s.symbol_data(symbol_index{0}));
    ASSERT_TRUE(s.is_symbol_set(symbol_index{1}));
    EXPECT_EQ((uint8_t*)0x1100, s.symbol_data(symbol_index{1}));
}
