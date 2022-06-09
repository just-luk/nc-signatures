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
#include <kodo/perpetual/encoder.hpp>

#include <kodo/perpetual/generator/random_uniform.hpp>
#include <kodo/perpetual/offset/random_uniform.hpp>

#include <algorithm>
#include <random>

#include "../helper/combine.hpp"

#include <gtest/gtest.h>

static void test_perpetual_code(kodo::perpetual::width width_type,
                                std::size_t symbol_bytes,
                                std::size_t block_bytes)
{

    SCOPED_TRACE(testing::Message() << "symbol_bytes: " << symbol_bytes);
    SCOPED_TRACE(testing::Message() << "block_bytes: " << block_bytes);

    kodo::perpetual::encoder encoder(width_type);
    kodo::perpetual::decoder decoder(width_type);
    kodo::perpetual::generator::random_uniform generator(width_type);

    kodo::perpetual::offset::random_uniform random_offset;
    decoder.configure(block_bytes, symbol_bytes);
    encoder.configure(block_bytes, symbol_bytes);

    // test logging
    auto test_logging = [](auto& codec)
    {
        EXPECT_FALSE(codec.is_log_enabled());
        codec.enable_log(
            [](const auto& name, const auto& msg, void* data)
            {
                (void)name;
                (void)msg;
                (void)data;
            },
            nullptr);
        EXPECT_TRUE(codec.is_log_enabled());
        codec.disable_log();
        EXPECT_FALSE(codec.is_log_enabled());
        EXPECT_EQ("", codec.log_name());
        codec.set_log_name("some name");
        EXPECT_EQ("some name", codec.log_name());
    };
    test_logging(decoder);
    test_logging(encoder);

    EXPECT_EQ(encoder.symbols(), decoder.symbols());
    std::size_t symbols = encoder.symbols();

    random_offset.configure(symbols);

    EXPECT_EQ(symbols, decoder.symbols());
    EXPECT_EQ(symbol_bytes, decoder.symbol_bytes());
    EXPECT_EQ(block_bytes, decoder.block_bytes());

    EXPECT_EQ(symbols, encoder.symbols());
    EXPECT_EQ(symbol_bytes, encoder.symbol_bytes());
    EXPECT_EQ(block_bytes, encoder.block_bytes());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::vector<uint8_t> data_out(encoder.block_bytes());

    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());
    decoder.set_symbols_storage(data_out.data());

    std::vector<uint8_t> symbol(encoder.symbol_bytes());

    while (!decoder.is_complete())
    {
        std::size_t offset = random_offset.offset();

        uint64_t coefficients = generator.generate(rand());

        encoder.encode_symbol(symbol.data(), coefficients, offset);
        decoder.decode_symbol(symbol.data(), coefficients, offset);
    }
    EXPECT_EQ(data_in, data_out);
}

TEST(perpetual_test_perpetual, api)
{
    std::vector<kodo::perpetual::width> widths{
        kodo::perpetual::width::_64, kodo::perpetual::width::_32,
        kodo::perpetual::width::_16, kodo::perpetual::width::_8};
    std::vector<std::size_t> symbol_bytes{1250U, 100U};
    std::vector<std::size_t> block_bytes{1234567U};

    helper::combine(
        [](auto v) {
            test_perpetual_code(std::get<0>(v), std::get<1>(v), std::get<2>(v));
        },
        widths, symbol_bytes, block_bytes);

    // Corner case only one symbol
    test_perpetual_code(kodo::perpetual::width::_32, 1250U, 1250U);
}

TEST(perpetual_test_perpetual, tiny_block)
{
    test_perpetual_code(kodo::perpetual::width::_8, 10U, 2000U);
}
