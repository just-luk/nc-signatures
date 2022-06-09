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

#include <kodo/fulcrum/decoder.hpp>

#include <algorithm>

#include <gtest/gtest.h>

TEST(fulcrum_test_decoder, field)
{
    kodo::fulcrum::decoder decoder;

    {
        decoder = kodo::fulcrum::decoder(kodo::finite_field::binary4);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary4);
    }
    {
        decoder = kodo::fulcrum::decoder(kodo::finite_field::binary8);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary8);
    }
    {
        decoder = kodo::fulcrum::decoder(kodo::finite_field::binary16);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary16);
    }
}

TEST(fulcrum_test_decoder, configure)
{
    kodo::fulcrum::decoder decoder{kodo::finite_field::binary8};

    std::size_t symbol_bytes = 1400;
    std::size_t symbols = 23;
    std::size_t expected_block_bytes = 32200;
    std::size_t expansion = 3;

    decoder.configure(symbols, symbol_bytes, expansion);

    EXPECT_EQ(symbols, decoder.symbols());
    EXPECT_EQ(expansion, decoder.expansion());
    EXPECT_EQ(symbol_bytes, decoder.symbol_bytes());
    EXPECT_EQ(expected_block_bytes, decoder.block_bytes());
}
