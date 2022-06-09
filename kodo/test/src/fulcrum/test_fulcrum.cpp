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
#include <kodo/fulcrum/encoder.hpp>
#include <kodo/fulcrum/generator/random_uniform.hpp>

#include "../helper/fix_granularity.hpp"
#include "../helper/get_value.hpp"
#include "../helper/random_symbol_bytes.hpp"
#include "../helper/random_symbols.hpp"

#include <algorithm>
#include <cstdlib>

#include <gtest/gtest.h>

static void run_test_basic_api(kodo::finite_field field, std::size_t symbols,
                               std::size_t symbol_bytes, std::size_t expansion)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_bytes = " << symbol_bytes);
    SCOPED_TRACE(testing::Message() << "expansion = " << expansion);

    int seed = rand();
    SCOPED_TRACE(testing::Message() << "seed = " << seed);
    srand(seed);

    kodo::fulcrum::encoder encoder(field);
    kodo::fulcrum::decoder decoder(field);
    kodo::fulcrum::generator::random_uniform generator;

    encoder.configure(symbols, symbol_bytes, expansion);
    decoder.configure(symbols, symbol_bytes, expansion);
    generator.configure(encoder.symbols(), encoder.expansion());

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

    EXPECT_EQ(field, encoder.field());
    EXPECT_EQ(symbols, encoder.symbols());
    EXPECT_EQ(expansion, encoder.expansion());
    EXPECT_EQ(symbol_bytes, encoder.symbol_bytes());

    EXPECT_EQ(field, decoder.field());
    EXPECT_EQ(symbols, decoder.symbols());
    EXPECT_EQ(expansion, decoder.expansion());
    EXPECT_EQ(symbol_bytes, decoder.symbol_bytes());

    std::size_t block_bytes = symbols * symbol_bytes;
    EXPECT_EQ(block_bytes, encoder.block_bytes());
    EXPECT_EQ(block_bytes, decoder.block_bytes());

    EXPECT_EQ(0U, encoder.rank());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);
    encoder.set_symbols_storage(data_in.data());

    EXPECT_EQ(symbols, encoder.rank());

    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    EXPECT_EQ(0U, decoder.rank());
    EXPECT_FALSE(decoder.is_complete());

    // Initially all symbols should be missing
    for (std::size_t i = 0; i < symbols; ++i)
    {
        SCOPED_TRACE(testing::Message() << "index = " << i);
        EXPECT_FALSE(decoder.is_symbol_decoded(i));
        EXPECT_FALSE(decoder.is_symbol_pivot(i));
    }

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::size_t received_symbols = 0;
    std::size_t max_iterations = 10000000;
    while (!decoder.is_complete())
    {
        ASSERT_NE(0U, --max_iterations);
        // Write random bytes to the coefficient vector
        generator.generate(coefficients.data());

        encoder.encode_symbol(symbol.data(), coefficients.data());
        if (rand() % 100 < 1)
            continue;
        decoder.decode_symbol(symbol.data(), coefficients.data());
        // Check that the decoder can use the encoded symbols.
        ASSERT_LT(++received_symbols, (symbols + 2) * 2);
    }

    // Now all symbols should be decoded an pivots
    for (std::size_t i = 0; i < symbols; ++i)
    {
        SCOPED_TRACE(testing::Message() << "index = " << i);
        EXPECT_TRUE(decoder.is_symbol_decoded(i));
        EXPECT_TRUE(decoder.is_symbol_pivot(i));
    }

    ASSERT_TRUE(decoder.is_complete());
    ASSERT_EQ(data_in, data_out);

    const uint8_t* data_in_ptr = data_in.data();
    for (std::size_t i = 0; i < symbols; ++i)
    {
        std::vector<uint8_t> expected_symbol(data_in_ptr,
                                             data_in_ptr + symbol_bytes);
        data_in_ptr += symbol_bytes;

        std::vector<uint8_t> symbol(decoder.symbol_data(i),
                                    decoder.symbol_data(i) + symbol_bytes);
        EXPECT_EQ(expected_symbol, symbol);
    }

    // Reconfigure the coder parameters for a second phase
    encoder.configure(2 * symbols, 2 * symbol_bytes, 2 * expansion);
    decoder.configure(2 * symbols, 2 * symbol_bytes, 2 * expansion);

    // The encoder and decoder must be in a clean state after reconfiguration
    EXPECT_EQ(2 * symbols, encoder.symbols());
    EXPECT_EQ(2 * symbol_bytes, encoder.symbol_bytes());
    EXPECT_EQ(2 * expansion, encoder.expansion());
    EXPECT_EQ(2 * symbols, decoder.symbols());
    EXPECT_EQ(2 * symbol_bytes, decoder.symbol_bytes());
    EXPECT_EQ(2 * expansion, decoder.expansion());

    EXPECT_FALSE(decoder.is_complete());
    EXPECT_EQ(0U, decoder.rank());
}

/// Helper function that invokes the test_basic_api using a number of
/// different finite fields
static void test_basic_api(std::size_t symbols, std::size_t symbol_bytes,
                           std::size_t expansion)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_basic_api(kodo::finite_field::binary4, symbols, symbol_bytes,
                           expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_basic_api(kodo::finite_field::binary8, symbols, symbol_bytes,
                           expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_basic_api(kodo::finite_field::binary16, symbols,
                           helper::fix_granularity(symbol_bytes, 2), expansion);
    }
}

/// Tests the basic API functionality: basic encoding and decoding
TEST(test_fulcrum, test_basic_api)
{
    test_basic_api(1, 1600, 1);
    test_basic_api(2, 1600, 2);
    test_basic_api(32, 1600, 4);

    std::size_t symbols = helper::random_symbols(1, 100);
    std::size_t symbol_bytes = helper::random_symbol_bytes();
    std::size_t expansion = helper::random_symbols(1, 100);
    test_basic_api(symbols, symbol_bytes, expansion);
}

static void run_test_systematic(kodo::finite_field field, std::size_t symbols,
                                std::size_t symbol_bytes, std::size_t expansion)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_bytes = " << symbol_bytes);
    SCOPED_TRACE(testing::Message() << "expansion = " << expansion);

    int seed = rand();
    SCOPED_TRACE(testing::Message() << "seed = " << seed);
    srand(seed);

    kodo::fulcrum::encoder encoder(field);
    kodo::fulcrum::decoder decoder(field);
    kodo::fulcrum::generator::random_uniform generator;

    encoder.configure(symbols, symbol_bytes, expansion);
    decoder.configure(symbols, symbol_bytes, expansion);
    generator.configure(encoder.symbols(), encoder.expansion());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);
    encoder.set_symbols_storage(data_in.data());

    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    EXPECT_EQ(0U, decoder.rank());
    EXPECT_FALSE(decoder.is_complete());

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::size_t systematic_counter = 0;

    std::size_t received_symbols = 0;
    std::size_t max_iterations = 10000000;
    while (!decoder.is_complete())
    {
        ASSERT_NE(0U, --max_iterations);
        if (systematic_counter != symbols)
        {
            encoder.encode_systematic_symbol(symbol.data(), systematic_counter);

            if (rand() % 100 >= 1)
            {
                decoder.decode_systematic_symbol(symbol.data(),
                                                 systematic_counter);
                ASSERT_LT(++received_symbols, (symbols + 2) * 2);
            }
            systematic_counter += 1;
        }
        else
        {
            generator.generate(coefficients.data());
            encoder.encode_symbol(symbol.data(), coefficients.data());

            if (rand() % 100 < 1)
                continue;

            decoder.decode_symbol(symbol.data(), coefficients.data());
            ASSERT_LT(++received_symbols, (symbols + 2) * 2);
        }
    }

    ASSERT_TRUE(decoder.is_complete());

    ASSERT_EQ(data_in, data_out);
}

/// Helper function that invokes the test_basic_api using a number of
/// different finite fields
static void test_systematic(std::size_t symbols, std::size_t symbol_bytes,
                            std::size_t expansion)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_systematic(kodo::finite_field::binary4, symbols, symbol_bytes,
                            expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_systematic(kodo::finite_field::binary8, symbols, symbol_bytes,
                            expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_systematic(kodo::finite_field::binary16, symbols,
                            helper::fix_granularity(symbol_bytes, 2),
                            expansion);
    }
}

/// Tests the basic API functionality: basic encoding and decoding
TEST(test_fulcrum, test_systematic)
{
    test_systematic(1, 1600, 1);
    test_systematic(2, 1600, 2);
    test_systematic(32, 1600, 4);

    std::size_t symbols = helper::random_symbols(1, 100);
    std::size_t symbol_bytes = helper::random_symbol_bytes();
    std::size_t expansion = helper::random_symbols(1, 100);
    test_systematic(symbols, symbol_bytes, expansion);
}

static void run_test_on_the_fly(kodo::finite_field field, std::size_t symbols,
                                std::size_t symbol_bytes, std::size_t expansion)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_bytes = " << symbol_bytes);
    SCOPED_TRACE(testing::Message() << "expansion = " << expansion);

    int seed = rand();
    SCOPED_TRACE(testing::Message() << "seed = " << seed);
    srand(seed);

    kodo::fulcrum::encoder encoder(field);
    kodo::fulcrum::decoder decoder(field);
    kodo::fulcrum::generator::random_uniform generator;

    encoder.configure(symbols, symbol_bytes, expansion);
    decoder.configure(symbols, symbol_bytes, expansion);
    generator.configure(encoder.symbols(), encoder.expansion());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);
    std::size_t data_in_offset = 0;

    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    EXPECT_EQ(0U, decoder.rank());
    EXPECT_FALSE(decoder.is_complete());

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // interval at which symbols are added to the encoder.
    std::size_t interval = 2 + rand() % 4;

    std::size_t received_symbols = 0;
    std::size_t max_iterations = 10000000;
    while (!decoder.is_complete())
    {
        ASSERT_NE(0U, --max_iterations);

        if (max_iterations % interval == 0 && encoder.rank() != symbols)
        {
            encoder.set_symbol_storage(data_in.data() + data_in_offset,
                                       encoder.rank());
            data_in_offset += symbol_bytes;
        }

        if (encoder.rank() == 0)
            continue;

        // Write random bytes to the coefficient vector
        generator.generate_partial(coefficients.data(), encoder.inner_rank());

        encoder.encode_symbol(symbol.data(), coefficients.data());
        if (rand() % 100 < 1)
            continue;
        decoder.decode_symbol(symbol.data(), coefficients.data());
        ASSERT_LT(++received_symbols, ((symbols + 2) * interval) * 2);
    }

    ASSERT_TRUE(decoder.is_complete());

    ASSERT_EQ(data_in, data_out);
}

/// Helper function that invokes the test_basic_api using a number of
/// different finite fields
static void test_on_the_fly(std::size_t symbols, std::size_t symbol_bytes,
                            std::size_t expansion)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_on_the_fly(kodo::finite_field::binary, symbols, symbol_bytes,
                            expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_on_the_fly(kodo::finite_field::binary4, symbols, symbol_bytes,
                            expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_on_the_fly(kodo::finite_field::binary8, symbols, symbol_bytes,
                            expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_on_the_fly(kodo::finite_field::binary16, symbols,
                            helper::fix_granularity(symbol_bytes, 2),
                            expansion);
    }
}

/// Tests the basic API functionality: basic encoding and decoding
TEST(test_fulcrum, test_on_the_fly)
{
    test_on_the_fly(5, 1600, 2);
    test_on_the_fly(32, 1600, 4);

    std::size_t symbols = helper::random_symbols(1, 100);
    std::size_t symbol_bytes = helper::random_symbol_bytes();
    std::size_t expansion = helper::random_symbols(1, 100);
    test_on_the_fly(symbols, symbol_bytes, expansion);
}

static void run_test_recoding(kodo::finite_field field, std::size_t symbols,
                              std::size_t symbol_bytes, std::size_t expansion)
{
    SCOPED_TRACE(testing::Message() << "symbols = " << symbols);
    SCOPED_TRACE(testing::Message() << "symbol_bytes = " << symbol_bytes);
    SCOPED_TRACE(testing::Message() << "expansion = " << expansion);

    int seed = rand();
    SCOPED_TRACE(testing::Message() << "seed = " << seed);
    srand(seed);

    kodo::fulcrum::encoder encoder(field);
    kodo::fulcrum::decoder recoder(field);
    kodo::fulcrum::decoder decoder(field);
    kodo::fulcrum::generator::random_uniform generator;

    encoder.configure(symbols, symbol_bytes, expansion);
    recoder.configure(symbols, symbol_bytes, expansion);
    decoder.configure(symbols, symbol_bytes, expansion);
    generator.configure(encoder.symbols(), encoder.expansion());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);
    std::size_t data_in_offset = 0;

    std::vector<uint8_t> data_out_1(recoder.block_bytes());
    recoder.set_symbols_storage(data_out_1.data());

    std::vector<uint8_t> data_out_2(decoder.block_bytes());
    decoder.set_symbols_storage(data_out_2.data());

    EXPECT_EQ(0U, decoder.rank());
    EXPECT_FALSE(decoder.is_complete());

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
    std::vector<uint8_t> recoding_coefficients(
        generator.max_coefficients_bytes());

    // interval at which symbols are added to the encoder.
    std::size_t interval = 2 + rand() % 6;

    std::size_t received_symbols = 0;
    std::size_t max_iterations = 10000000;
    while (!decoder.is_complete())
    {
        ASSERT_NE(0U, --max_iterations);

        if (max_iterations % interval == 0 && encoder.rank() != symbols)
        {
            encoder.set_symbol_storage(data_in.data() + data_in_offset,
                                       encoder.rank());
            data_in_offset += symbol_bytes;
        }

        if (encoder.rank() == 0)
            continue;

        // Write random bytes to the coefficient vector
        generator.generate_partial(coefficients.data(), encoder.inner_rank());

        encoder.encode_symbol(symbol.data(), coefficients.data());
        if (rand() % 100 >= 1)
            recoder.decode_symbol(symbol.data(), coefficients.data());

        if (recoder.rank() == 0)
            continue;

        // Write random bytes to the recoding coefficient vector
        generator.generate_recode(recoding_coefficients.data(), recoder);

        recoder.recode_symbol(symbol.data(), coefficients.data(),
                              recoding_coefficients.data());
        if (rand() % 100 < 1)
            continue;
        decoder.decode_symbol(symbol.data(), coefficients.data());
        ASSERT_LT(++received_symbols, ((symbols + 2) * interval) * 2);
    }

    ASSERT_TRUE(recoder.is_complete());
    ASSERT_TRUE(decoder.is_complete());

    ASSERT_EQ(data_in, data_out_1);
    ASSERT_EQ(data_in, data_out_2);
}

/// Helper function that invokes the test_basic_api using a number of
/// different finite fields
static void test_recoding(std::size_t symbols, std::size_t symbol_bytes,
                          std::size_t expansion)
{
    {
        SCOPED_TRACE(testing::Message() << "field = binary");
        run_test_recoding(kodo::finite_field::binary, symbols, symbol_bytes,
                          expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary4");
        run_test_recoding(kodo::finite_field::binary4, symbols, symbol_bytes,
                          expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary8");
        run_test_recoding(kodo::finite_field::binary8, symbols, symbol_bytes,
                          expansion);
    }

    {
        SCOPED_TRACE(testing::Message() << "field = binary16");
        run_test_recoding(kodo::finite_field::binary16, symbols,
                          helper::fix_granularity(symbol_bytes, 2), expansion);
    }
}

/// Tests the basic API functionality: basic encoding and decoding
TEST(test_fulcrum, test_recoding)
{
    test_recoding(1, 1600, 1);
    test_recoding(2, 1600, 2);
    test_recoding(32, 1600, 4);
    std::size_t symbols = helper::random_symbols(1, 100);
    std::size_t symbol_bytes = helper::random_symbol_bytes();
    std::size_t expansion = helper::random_symbols(1, 100);
    test_recoding(symbols, symbol_bytes, expansion);
}
