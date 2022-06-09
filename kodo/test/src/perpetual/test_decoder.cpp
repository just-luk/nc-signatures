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

#include <kodo/perpetual/decoder.hpp>

#include <algorithm>

#include <gtest/gtest.h>

void test_configure(kodo::perpetual::width width_type, std::size_t block_bytes,
                    std::size_t symbol_bytes, std::size_t outer_interval,
                    std::size_t outer_segments, double mapping_threshold)
{
    kodo::perpetual::decoder decoder(width_type);
    decoder.configure(block_bytes, symbol_bytes, outer_interval, outer_segments,
                      mapping_threshold);

    auto width = decoder.width();
    auto zero_padding = width - 1;
    // Ceil division
    auto data_symbols =
        block_bytes / symbol_bytes + (block_bytes % symbol_bytes != 0);
    auto outer_symbols = data_symbols / decoder.outer_interval() +
                         (data_symbols % decoder.outer_interval() != 0);

    std::size_t symbols = zero_padding + data_symbols + outer_symbols;

    EXPECT_EQ(symbols, decoder.symbols());
    EXPECT_EQ(data_symbols, decoder.data_symbols());
    EXPECT_EQ(outer_symbols, decoder.outer_symbols());
    EXPECT_EQ(symbol_bytes, decoder.symbol_bytes());
    EXPECT_EQ(width, decoder.width());
    EXPECT_EQ(block_bytes, decoder.block_bytes());
    EXPECT_EQ(outer_interval, decoder.outer_interval());
    EXPECT_EQ(outer_segments, decoder.outer_segments());
    EXPECT_DOUBLE_EQ(mapping_threshold, decoder.mapping_threshold());
}

TEST(perpetual_test_decoder, configure)
{
    test_configure(kodo::perpetual::width::_8, 32000, 1400, 6, 10, 0.6);
    test_configure(kodo::perpetual::width::_16, 32000, 1400, 6, 10, 0.6);
    test_configure(kodo::perpetual::width::_32, 32000, 1400, 6, 10, 0.6);
    test_configure(kodo::perpetual::width::_64, 32000, 1400, 6, 10, 0.6);
}
