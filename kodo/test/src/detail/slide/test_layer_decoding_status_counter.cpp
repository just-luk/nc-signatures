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

#include <kodo/detail/slide/layer_decoding_status_counter.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    stub::function<void()> push_symbol;
    stub::function<symbol_range()> stream_range;
    stub::function<bool(symbol_index)> is_symbol_missing;
    stub::function<bool(symbol_index)> is_symbol_partially_decoded;
    stub::function<bool(symbol_index)> is_symbol_decoded;
    stub::function<uint8_t*()> pop_symbol;
    stub::function<void(symbol_index)> set_symbol_partially_decoded;
    stub::function<void(symbol_index)> set_symbol_decoded;
};
}

TEST(detail_slide_test_layer_decoding_status_counter, basic_api)
{
    slide::layer_decoding_status_counter<dummy> stack;
    dummy& d = stack;

    // Pushing symbols should increase the missing count
    stack.push_symbol();
    stack.push_symbol();
    stack.push_symbol();

    EXPECT_EQ(symbol_count{3U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_decoded());

    // Pop a missing symbol
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(true);
    d.pop_symbol.set_return((uint8_t*)0xdeadbeef);
    stack.pop_symbol();

    EXPECT_EQ(symbol_count{2U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_decoded());

    // Push a symbol again
    stack.push_symbol();

    EXPECT_EQ(symbol_count{3U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_decoded());

    // Set a partially decoded
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(true);
    stack.set_symbol_partially_decoded(symbol_index{4U});

    EXPECT_EQ(symbol_count{2U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_decoded());

    // Set missing symbol decoded
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(true);
    d.is_symbol_decoded.set_return(false);

    stack.set_symbol_decoded(symbol_index{4U});

    EXPECT_EQ(symbol_count{1U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_decoded());

    // Set partially decoded symbol decoded
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(false);
    d.is_symbol_partially_decoded.set_return(true);
    d.is_symbol_decoded.set_return(false);

    stack.set_symbol_decoded(symbol_index{4U});

    EXPECT_EQ(symbol_count{1U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{2U}, stack.symbols_decoded());

    // Set a partially decoded symbol
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(true);
    stack.set_symbol_partially_decoded(symbol_index{4U});

    EXPECT_EQ(symbol_count{0U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{2U}, stack.symbols_decoded());

    // Pop decoded symbol
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(false);
    d.is_symbol_partially_decoded.set_return(false);
    d.is_symbol_decoded.set_return(true);
    stack.pop_symbol();

    EXPECT_EQ(symbol_count{0U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_decoded());

    // Pop partially decoded symbol
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(false);
    d.is_symbol_partially_decoded.set_return(true);
    d.is_symbol_decoded.set_return(false);
    stack.pop_symbol();

    EXPECT_EQ(symbol_count{0U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{1U}, stack.symbols_decoded());

    // Pop decoded symbol
    d.stream_range.set_return(symbol_range{symbol_index{3U}, symbol_index{6U}});
    d.is_symbol_missing.set_return(false);
    d.is_symbol_partially_decoded.set_return(false);
    d.is_symbol_decoded.set_return(true);
    stack.pop_symbol();

    EXPECT_EQ(symbol_count{0U}, stack.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, stack.symbols_decoded());
}
