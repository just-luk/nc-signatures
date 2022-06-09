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

#include <kodo/detail/perpetual/stack_encoder.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_perpetual_test_stack_encoder, tiny_block)
{
    byte_count block_bytes{10};
    byte_count symbol_bytes{20};
    symbol_count outer_interval{8};
    std::size_t outer_segments{8};

    typename perpetual::stack_encoder<uint32_t>::config encoder_config;
    encoder_config.symbol_bytes = symbol_bytes;
    encoder_config.block_bytes = block_bytes;
    encoder_config.outer_interval = outer_interval;
    encoder_config.outer_segments = outer_segments;

    perpetual::stack_encoder<uint32_t> encoder;
    encoder.configure(encoder_config);

    EXPECT_EQ(symbol_count{1U}, encoder.data_symbols());
    EXPECT_EQ(block_bytes, encoder.block_bytes());
    EXPECT_EQ(symbol_bytes, encoder.max_symbol_bytes());
    EXPECT_EQ(block_bytes, encoder.symbol_bytes(encoder.last_symbol()));
}
