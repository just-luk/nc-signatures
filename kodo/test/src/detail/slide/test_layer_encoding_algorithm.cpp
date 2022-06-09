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

#include <kodo/detail/slide/layer_encoding_algorithm.hpp>

#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy_layer
{

    stub::function<symbol_range()> stream_range;
    stub::function<bool(symbol_range, symbol_index)> in_range;
    stub::function<byte_count(symbol_index)> symbol_bytes;
    stub::function<const uint8_t*(symbol_index)> symbol_data;
};
}

TEST(detail_slide_test_detail_layer_encoding_algorithm,
     encode_systematic_symbol)
{
    slide::layer_encoding_algorithm<dummy_layer> stack;

    stack.stream_range.set_return(to_symbol_range(2, 3));
    stack.in_range.set_return(true);
    stack.symbol_bytes.set_return(byte_count{1U});

    std::vector<std::vector<uint8_t>> symbols{{'a'}, {'b'}, {'c'}};

    stack.symbol_data
        .set_return(symbols[0].data(), symbols[1].data(), symbols[2].data())
        .no_repeat();

    std::vector<std::vector<uint8_t>> output{{'0'}, {'0'}, {'0'}};

    stack.encode_systematic_symbol(output[0].data(), symbol_index{2});
    stack.encode_systematic_symbol(output[1].data(), symbol_index{3});
    stack.encode_systematic_symbol(output[2].data(), symbol_index{4});

    EXPECT_EQ(symbols, output);
}
