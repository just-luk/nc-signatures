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

#include <kodo/detail/slide/layer_systematic_decoding_algorithm.hpp>

#include <kodo/detail/byte_count.hpp>
#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy_vector
{
};
struct dummy_layer
{
    stub::function<bool(symbol_index)> in_stream;
    stub::function<bool(symbol_index)> is_symbol_decoded;
    stub::function<bool(symbol_index)> is_symbol_partially_decoded;
    stub::function<dummy_vector(symbol_range)> allocate_coefficients_vector;
    stub::function<void(dummy_vector, symbol_index, uint32_t)> set_value;
    stub::function<void(symbol_index, dummy_vector)> set_coefficients_vector;
    stub::function<void(symbol_index)> set_symbol_decoded;
    stub::function<void(symbol_index, uint8_t*, byte_count)> set_symbol_storage;
    stub::function<uint8_t*(uint8_t*, byte_count, symbol_index)> swap_decode;
    stub::function<void(symbol_index)> backward_substitute;
    stub::function<byte_count()> max_symbol_bytes;
    stub::function<symbol_index()> stream_upper_bound;
};
}

TEST(detail_slide_test_detail_layer_systematic_decoding_algorithm,
     decode_systematic_symbol)
{
    slide::layer_systematic_decoding_algorithm<dummy_layer> stack;

    stack.in_stream.set_return(true);
    stack.is_symbol_decoded.set_return(false);
    stack.is_symbol_partially_decoded.set_return(false);
    stack.stream_upper_bound.set_return(symbol_index{1});

    dummy_vector v;
    stack.allocate_coefficients_vector.set_return(v);
    std::vector<std::vector<uint8_t>> symbols{
        {'f', 'w'}, {'t', 't'}, {'w', 'f'}};

    stack.max_symbol_bytes.set_return(byte_count{2});

    std::vector<std::vector<uint8_t>> input{
        {'a', 'b'}, {'c', '\0'}, {'d', '\0'}};

    stack.decode_systematic_symbol(input[0].data(), byte_count{2},
                                   symbol_index{2});
    stack.decode_systematic_symbol(input[1].data(), byte_count{1},
                                   symbol_index{3});
    stack.decode_systematic_symbol(input[2].data(), byte_count{1},
                                   symbol_index{4});

    EXPECT_TRUE(stack.set_symbol_storage.expect_calls()
                    .with(symbol_index{2}, input[0].data(), byte_count{2})
                    .with(symbol_index{3}, input[1].data(), byte_count{1})
                    .with(symbol_index{4}, input[2].data(), byte_count{1})
                    .to_bool());
}
