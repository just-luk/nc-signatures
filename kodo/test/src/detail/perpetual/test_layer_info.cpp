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

#include <kodo/detail/perpetual/layer_info.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    struct config
    {
        byte_count symbol_bytes;
        byte_count block_bytes;
    };

    template <class Config>
    void configure(const Config&)
    {
    }
};

struct stack : public perpetual::layer_info<uint32_t, dummy>
{
};

}

TEST(detail_perpetual_test_perpetual_layer_info, data)
{

    stack s;
    stack::config c;
    c.outer_interval = symbol_count{0};
    c.symbol_bytes = byte_count{100};
    c.block_bytes = byte_count{1000};
    c.outer_segments = 8U;

    s.configure(c);

    EXPECT_EQ(s.zero_symbols(), symbol_count{31});
    EXPECT_EQ(s.data_symbols(), symbol_count{10});
    EXPECT_EQ(s.outer_symbols(), symbol_count{0});
    EXPECT_EQ(s.outer_interval(), symbol_count{0});
    EXPECT_EQ(s.outer_segments(), 8U);
    EXPECT_EQ(s.outer_range(symbol_index{0}), to_symbol_range(1, 41));
}

TEST(detail_perpetual_test_perpetual_layer_info, outer)
{

    stack s;
    stack::config c;
    c.outer_interval = symbol_count{2};
    c.symbol_bytes = byte_count{100};
    c.block_bytes = byte_count{1000};
    c.outer_segments = 8U;

    s.configure(c);

    EXPECT_EQ(s.zero_symbols(), symbol_count{31});
    EXPECT_EQ(s.data_symbols(), symbol_count{10});
    EXPECT_EQ(s.outer_symbols(), symbol_count{5});
    EXPECT_EQ(s.outer_interval(), symbol_count{2});
    EXPECT_EQ(s.outer_segments(), 8U);
}

TEST(detail_perpetual_test_perpetual_layer_info, outer_slope)
{

    stack s;
    stack::config c;
    c.outer_interval = symbol_count{2};
    c.symbol_bytes = byte_count{100};
    c.block_bytes = byte_count{1000};
    // One segment means we only have 32 symbols in the outer code
    c.outer_segments = 1U;

    s.configure(c);

    EXPECT_EQ(s.zero_symbols(), symbol_count{31});
    EXPECT_EQ(s.data_symbols(), symbol_count{10});
    EXPECT_EQ(s.outer_symbols(), symbol_count{5});
    EXPECT_EQ(s.outer_interval(), symbol_count{2});
    EXPECT_EQ(s.outer_segments(), 1U);
    EXPECT_EQ(s.symbols(), symbol_count{46});

    EXPECT_EQ(s.outer_range(symbol_index{0}), to_symbol_range(1, 33));
    EXPECT_EQ(s.outer_range(symbol_index{1}), to_symbol_range(2, 34));
    EXPECT_EQ(s.outer_range(symbol_index{10}), to_symbol_range(11, 43));
    EXPECT_EQ(s.outer_range(symbol_index{20}), to_symbol_range(21, 46));
}
