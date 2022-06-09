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

#include <kodo/detail/block/layer_decoding_callback.hpp>

#include <kodo/detail/symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

namespace
{
struct dummy
{
    template <class Config>
    void configure(Config& config)
    {
        (void)config;
    }

    stub::function<kodo::detail::symbol_range()> symbol_range;
    stub::function<bool(kodo::detail::symbol_index)> is_symbol_decoded;
    stub::function<void(kodo::detail::symbol_index)> set_symbol_decoded;
    stub::function<void()> set_symbols_decoded;
};
}

TEST(detail_block_test_layer_decoding_callback, decode)
{

    using namespace kodo::detail;

    block::layer_decoding_callback<dummy> stack;
    dummy& d = stack;

    EXPECT_FALSE(stack.is_on_symbol_decoded_set());
    EXPECT_EQ(0U, d.set_symbols_decoded.calls());
    stack.set_symbols_decoded();
    EXPECT_EQ(1U, d.set_symbols_decoded.calls());

    kodo::symbol_decoded_callback callback = [](uint64_t index, void* user_data)
    {
        auto indices = (std::vector<uint64_t>*)user_data;
        indices->push_back(index);
    };
    std::vector<uint64_t> indices;

    stack.on_symbol_decoded(callback, &indices);

    EXPECT_TRUE(stack.is_on_symbol_decoded_set());

    stack.set_symbol_decoded(symbol_index{0});

    EXPECT_EQ(1U, indices.size());
    EXPECT_EQ(0U, indices[0]);

    d.symbol_range.set_return(symbol_range{symbol_index{0}, symbol_index{5}});

    // Checked for index 0, 1, 2, 3, 4
    d.is_symbol_decoded.set_return(true, false, false, false, false);

    stack.set_symbols_decoded();

    EXPECT_EQ(5U, indices.size());
    EXPECT_EQ(0U, indices[0]);
    EXPECT_EQ(1U, indices[1]);
    EXPECT_EQ(2U, indices[2]);
    EXPECT_EQ(3U, indices[3]);
    EXPECT_EQ(4U, indices[4]);

    // empty config
    struct
    {
    } config;

    stack.configure(config);
    EXPECT_FALSE(stack.is_on_symbol_decoded_set());
}
