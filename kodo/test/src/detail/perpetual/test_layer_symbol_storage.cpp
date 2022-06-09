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

#include <kodo/detail/perpetual/layer_symbol_mapping.hpp>
#include <kodo/detail/perpetual/layer_symbol_storage.hpp>

#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct config
{
    byte_count symbol_bytes;
    byte_count block_bytes;
};

struct dummy
{
    void configure(const config&)
    {
    }

    stub::function<symbol_count()> symbols;
    stub::function<symbol_count()> zero_symbols;
    stub::function<symbol_count()> data_symbols;
    stub::function<symbol_index()> last_symbol;

    stub::function<symbol_count()> outer_symbols;
    stub::function<symbol_count()> outer_interval;
};

// clang-format off
struct stack : public
    perpetual::layer_symbol_storage<uint8_t*,
    perpetual::layer_symbol_mapping<dummy>>
{
};
}
// clang-format on

TEST(detail_perpetual_test_layer_perpetual_symbol_storage, api)
{
    stack s;
    s.symbols.set_return(symbol_count{9});
    s.zero_symbols.set_return(symbol_count{4});
    s.data_symbols.set_return(symbol_count{5});
    s.outer_symbols.set_return(symbol_count{0});
    s.outer_interval.set_return(symbol_count{0});
    s.last_symbol.set_return(symbol_index{8});

    byte_count symbol_bytes{100};
    byte_count block_bytes{100 * 1000};

    config c{symbol_bytes, block_bytes};
    s.configure(c);

    EXPECT_EQ(symbol_bytes, s.max_symbol_bytes());

    s.set_symbols_storage((uint8_t*)1000);

    // The first symbols are zero padding e.g. if zero symbols is 4 then
    // we have:
    //   0  1  2  3  4  5  6 ...
    //  z0 z1 z2 z3 s0 s1 s2 ... etc.

    EXPECT_EQ((uint8_t*)1000, s.symbol_data(symbol_index{4}));
    EXPECT_EQ((uint8_t*)1100, s.symbol_data(symbol_index{5}));
    EXPECT_EQ((uint8_t*)1200, s.symbol_data(symbol_index{6}));

    EXPECT_EQ(block_bytes, s.block_bytes());
}

TEST(detail_perpetual_test_layer_perpetual_symbol_storage, last_symbol)
{
    stack s;
    s.symbols.set_return(symbol_count{13});
    s.zero_symbols.set_return(symbol_count{4});
    s.data_symbols.set_return(symbol_count{9});
    s.outer_symbols.set_return(symbol_count{0});
    s.outer_interval.set_return(symbol_count{0});
    s.last_symbol.set_return(symbol_index{12});

    byte_count symbol_bytes{100};
    byte_count block_bytes{895};

    config c{symbol_bytes, block_bytes};
    s.configure(c);

    EXPECT_EQ(symbol_bytes, s.max_symbol_bytes());

    s.set_symbols_storage((uint8_t*)1000);

    EXPECT_EQ(s.last_symbol(), symbol_index{12});
    EXPECT_EQ(s.symbol_bytes(s.last_symbol()), byte_count{95});
}

TEST(detail_perpetual_test_layer_perpetual_symbol_storage, check_pointers)
{
    // Symbols
    // Index:    0   1   2   3   4   5   6   7   8
    // Mapping: z0, z1, r0, s0, s1, s2, r1, s3, s4

    stack s;
    s.symbols.set_return(symbol_count{9});
    s.zero_symbols.set_return(symbol_count{2});
    s.data_symbols.set_return(symbol_count{5});
    s.outer_symbols.set_return(symbol_count{2});
    s.outer_interval.set_return(symbol_count{3});
    s.last_symbol.set_return(symbol_index{8});

    byte_count symbol_bytes{100};
    byte_count block_bytes{450};

    config c{symbol_bytes, block_bytes};
    s.configure(c);

    EXPECT_EQ(symbol_bytes, s.max_symbol_bytes());

    s.set_symbols_storage((uint8_t*)1000);

    EXPECT_EQ(s.last_symbol(), symbol_index{8});
    EXPECT_EQ(s.symbol_bytes(s.last_symbol()), byte_count{50});

    symbol_range symbols = to_symbol_range(0, 9);

    for (symbol_index i : symbols)
    {
        EXPECT_TRUE(s.symbol_data(i) != nullptr);
    }

    EXPECT_EQ(s.symbol_bytes(symbol_index{0}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{1}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{2}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{3}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{4}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{5}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{6}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{7}), byte_count{100});
    EXPECT_EQ(s.symbol_bytes(symbol_index{8}), byte_count{50});

    EXPECT_EQ(s.symbols_storage(), (uint8_t*)1000);

    EXPECT_NE((uint8_t*)1000, s.symbol_data(symbol_index{0})); // zero
    EXPECT_NE((uint8_t*)1000, s.symbol_data(symbol_index{1})); // zero
    EXPECT_NE((uint8_t*)1000, s.symbol_data(symbol_index{2})); // zero
    EXPECT_EQ((uint8_t*)1000, s.symbol_data(symbol_index{3}));
    EXPECT_EQ((uint8_t*)1100, s.symbol_data(symbol_index{4}));
    EXPECT_EQ((uint8_t*)1200, s.symbol_data(symbol_index{5}));
    EXPECT_NE((uint8_t*)1300, s.symbol_data(symbol_index{6})); // repair
    EXPECT_EQ((uint8_t*)1300, s.symbol_data(symbol_index{7}));
}
