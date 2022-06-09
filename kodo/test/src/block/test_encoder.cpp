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

#include <kodo/block/encoder.hpp>

#include <gtest/gtest.h>

TEST(block_test_encoder, field)
{
    kodo::block::encoder encoder;

    {
        encoder = kodo::block::encoder(kodo::finite_field::binary);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary);
    }
    {
        encoder = kodo::block::encoder(kodo::finite_field::binary4);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary4);
    }
    {
        encoder = kodo::block::encoder(kodo::finite_field::binary8);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary8);
    }
    {
        encoder = kodo::block::encoder(kodo::finite_field::binary16);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary16);
    }
}

TEST(block_test_encoder, configure)
{
    kodo::block::encoder encoder{kodo::finite_field::binary8};

    std::size_t symbol_bytes = 1400;
    std::size_t symbols = 23;
    std::size_t expected_block_bytes = 32200;

    encoder.configure(symbols, symbol_bytes);

    EXPECT_EQ(symbols, encoder.symbols());
    EXPECT_EQ(symbol_bytes, encoder.symbol_bytes());
    EXPECT_EQ(expected_block_bytes, encoder.block_bytes());
}

TEST(block_test_encoder, set_symbol_storage)
{
    kodo::block::encoder encoder{kodo::finite_field::binary8};

    std::size_t symbol_bytes = 1400;
    std::size_t symbols = 23;

    encoder.configure(symbols, symbol_bytes);
    EXPECT_EQ(0U, encoder.rank());
    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    encoder.set_symbol_storage(symbol.data(), 0);
    EXPECT_EQ(1U, encoder.rank());

    encoder.set_symbol_storage(symbol.data(), 2);
    EXPECT_EQ(2U, encoder.rank());

    encoder.set_symbol_storage(symbol.data(), 1);
    EXPECT_EQ(3U, encoder.rank());
}
