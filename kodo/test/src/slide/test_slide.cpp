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

#include <algorithm>
#include <cstring>
#include <deque>
#include <numeric>
#include <random>

#include <kodo/slide/decoder.hpp>
#include <kodo/slide/encoder.hpp>
#include <kodo/slide/generator/random_uniform.hpp>
#include <kodo/slide/rate_controller.hpp>
#include <kodo/to_string.hpp>

#include <gtest/gtest.h>

// This tests exercises the encoder and decoder API. In this case we let the
// window grow with the stream. Essentially this works similarly to a block
// code.
TEST(test_slide, basic_api)
{
    // Set the capacity of the decoder (this is the number of encoded symbols
    // that are used in the decoding process).
    std::size_t symbols = 100U;

    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;

    // The size of a symbol in bytes
    std::size_t max_symbol_bytes = 20U;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

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

    kodo::slide::generator::random_uniform generator(field);

    // Allocate some input symbols for the encoder
    std::vector<std::vector<uint8_t>> input_symbols(symbols);
    std::vector<std::vector<uint8_t>> output_symbols(symbols);

    // Initialize the input symbols
    for (auto& symbol : input_symbols)
    {
        symbol.resize(max_symbol_bytes);
        std::generate_n(symbol.data(), max_symbol_bytes, rand);
    }

    // Initialize the output symbols
    for (auto& symbol : output_symbols)
    {
        symbol.resize(max_symbol_bytes);
    }

    // Memory for the coding coefficients
    std::vector<uint8_t> coefficients = {2};

    // Make sure we will not hang on bugs that cause infinite loops
    std::size_t iterations = 0;

    while (decoder.symbols_decoded() < symbols)
    {
        ASSERT_TRUE(iterations < 1000);

        if (encoder.stream_symbols() < symbols)
        {
            // Randomly push a new symbol
            if (rand() % 2)
            {
                const uint8_t* symbol =
                    input_symbols[encoder.stream_symbols()].data();

                encoder.push_symbol(symbol, max_symbol_bytes);
                decoder.push_symbol();
            }
        }

        if (encoder.stream_symbols() == 0)
        {
            continue;
        }

        uint64_t seed = rand();
        generator.set_seed(seed);

        auto window = encoder.stream_range();
        coefficients.resize(generator.coefficients_bytes(window));
        generator.generate(coefficients.data(), window);

        uint8_t* symbol = output_symbols[decoder.rank()].data();

        encoder.encode_symbol(symbol, window, coefficients.data());

        decoder.decode_symbol(symbol, max_symbol_bytes, window,
                              coefficients.data());

        // Compute the rank from the pivots
        std::size_t rank = 0;
        for (std::size_t i = 0; i < decoder.stream_symbols(); ++i)
        {
            rank += decoder.is_symbol_pivot(i);
        }

        EXPECT_EQ(decoder.rank(), rank);

        ++iterations;
    }

    SCOPED_TRACE(testing::Message() << "iterations = " << iterations);

    for (std::size_t i = 0; i < symbols; ++i)
    {
        EXPECT_TRUE(decoder.is_symbol_decoded(i));
    }

    for (std::size_t i = 0; i < symbols; ++i)
    {
        SCOPED_TRACE(testing::Message() << "checking = " << i);

        const uint8_t* src = encoder.symbol_data(i);
        const uint8_t* dst = decoder.symbol_data(i);

        bool match = (0 == std::memcmp(src, dst, max_symbol_bytes));

        if (!match)
        {
            for (std::size_t j = 0; j < max_symbol_bytes; ++j)
            {
                std::cout << (std::size_t)src[j] << " = " << (std::size_t)dst[j]
                          << "\n";
            }

            std::cout << "input symbols (size " << input_symbols.size()
                      << ")\n";

            for (std::size_t j = 0; j < max_symbol_bytes; ++j)
            {
                std::cout << (std::size_t)input_symbols[0][j] << "\n";
            }
        }

        ASSERT_TRUE(match);
    }
}

// Test that everything works when sliding the window - also with losses. Since
// we only move the window and stream forward we may not decode all symbols.
TEST(test_slide, slide_window)
{
    // Set the capacity of the decoder (this is the number of encoded symbols
    // that are used in the decoding process).
    std::size_t decoder_symbols = 10U;

    // Set the window size (this is the number of symbols included in an
    // encoded symbol).
    std::size_t window_symbols = 5U;

    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;

    // The size of a symbol in bytes
    std::size_t max_symbol_bytes = 160U;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

    kodo::slide::generator::random_uniform generator(field);

    // Cache for all original source symbols added to the encoder - such that
    // we can check that they are decoded correctly.
    std::deque<std::vector<uint8_t>> source_symbols;

    // The memory used by the decoder
    std::vector<std::vector<uint8_t>> output_memory(decoder_symbols);
    std::deque<uint8_t*> output_symbols;

    for (auto& symbol : output_memory)
    {
        symbol.resize(max_symbol_bytes);
        output_symbols.push_back(symbol.data());
    }

    std::vector<uint8_t> coefficients;

    // Initialize our rate controller
    kodo::slide::rate_controller control(7, 3);

    // Make sure we will not hang on bugs that cause infinite loops
    std::size_t iterations = 0;

    // Counter for keeping track of the number of decoded symbols
    uint32_t decoded = 0;

    while (decoded < 100U)
    {
        // Check if somethings wrong
        ASSERT_TRUE(iterations < 10000U);

        // Manage the encoder's window
        if (!control.send_repair())
        {
            // Create a new source symbol
            source_symbols.emplace_back(max_symbol_bytes);
            uint8_t* symbol = source_symbols.back().data();
            std::generate_n(symbol, max_symbol_bytes, rand);

            if (encoder.stream_symbols() == window_symbols)
            {
                // If window is full - pop a symbol before pushing a new one
                encoder.pop_symbol();
            }

            encoder.push_symbol(symbol, max_symbol_bytes);
        }

        // Move the decoders's window / stream if needed
        while (decoder.stream_upper_bound() < encoder.stream_upper_bound())
        {
            // Add a symbol to the decoder
            decoder.push_symbol();

            // See if we need to pop something off
            if (decoder.stream_symbols() == decoder_symbols)
            {
                std::size_t lower_bound = decoder.stream_lower_bound();

                if (decoder.is_symbol_decoded(lower_bound))
                {
                    ++decoded;

                    // Compare with corresponding source symbol
                    uint8_t* src = source_symbols[lower_bound].data();
                    uint8_t* dst = decoder.symbol_data(lower_bound);

                    EXPECT_TRUE(std::memcmp(src, dst, max_symbol_bytes) == 0);
                }

                uint8_t* pop_symbol = decoder.pop_symbol();

                if (pop_symbol != nullptr)
                {
                    // Symbol was initialized
                    output_symbols.push_front(pop_symbol);
                }
            }

            // We should always have at least one output symbol ready
            ASSERT_TRUE(output_symbols.size() > 0);
        }

        ASSERT_TRUE(output_symbols.size() > 0);

        uint8_t* symbol = output_symbols.front();

        // Choose a seed for this encoding
        uint64_t seed = rand();

        // Encode a symbol
        kodo::slide::range window = encoder.stream_range();

        // Resize buffers
        coefficients.resize(generator.coefficients_bytes(window));

        generator.set_seed(seed);
        generator.generate(coefficients.data(), window);

        encoder.encode_symbol(symbol, window, coefficients.data());

        // Update loop state
        ++iterations;
        control.advance();

        if (rand() % 2)
        {
            // Simulate 50% packet loss
            continue;
        }

        symbol = decoder.decode_symbol(symbol, max_symbol_bytes, window,
                                       coefficients.data());

        if (symbol == nullptr)
        {
            output_symbols.pop_front();
        }
    }

    SCOPED_TRACE(testing::Message() << "decoded = " << decoded);
    SCOPED_TRACE(testing::Message() << "iterations = " << iterations);
}

void mix_coded_uncoded(kodo::finite_field field)
{
    SCOPED_TRACE(::testing::Message() << "field: " << kodo::to_string(field));
    // Set the capacity of the decoder (this is the number of encoded
    // symbols that are used in the decoding process).
    std::size_t decoder_symbols = 15U;

    // Set the window size (this is the number of symbols included in an
    // encoded symbol).
    std::size_t window_symbols = 5U;

    // The size of a symbol in bytes
    std::size_t max_symbol_bytes = 161U;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

    kodo::slide::generator::random_uniform generator(field);

    // Cache for all original source symbols added to the encoder - such
    // that we can check that they are decoded correctly.
    std::deque<std::vector<uint8_t>> source_symbols;

    // The memory used by the decoder
    std::vector<std::vector<uint8_t>> output_memory(decoder_symbols);
    std::deque<uint8_t*> output_symbols;

    for (auto& symbol : output_memory)
    {
        symbol.resize(max_symbol_bytes);
        output_symbols.push_back(symbol.data());
    }

    std::vector<uint8_t> coefficients;

    // Initialize our rate controller
    kodo::slide::rate_controller control(8, 3);

    // Make sure we will not hang on bugs that cause infinite loops
    std::size_t iterations = 0;

    // Counter for keeping track of the number of decoded symbols
    uint32_t decoded = 0;

    // Choose a seed for this encoding
    // std::random_device device;

    // Random generator
    std::mt19937 index_generator(rand());

    while (decoded < 100U)
    {
        // Check if somethings wrong
        ASSERT_TRUE(iterations < 10000U);

        // Manage the encoder's window
        if (!control.send_repair())
        {
            // Create a new source symbol
            source_symbols.emplace_back(max_symbol_bytes);
            uint8_t* symbol = source_symbols.back().data();
            std::generate_n(symbol, max_symbol_bytes, rand);

            if (encoder.stream_symbols() == window_symbols)
            {
                // If window is full - pop a symbol before pushing a new one
                encoder.pop_symbol();
            }

            encoder.push_symbol(symbol, max_symbol_bytes);
        }

        // Move the decoders's window / stream if needed
        while (decoder.stream_upper_bound() < encoder.stream_upper_bound())
        {
            // Add a symbol to the decoder
            decoder.push_symbol();

            // See if we need to pop something off
            if (decoder.stream_symbols() == decoder_symbols)
            {
                std::size_t lower_bound = decoder.stream_lower_bound();

                if (decoder.is_symbol_decoded(lower_bound))
                {
                    ++decoded;

                    // Compare with corresponding source symbol
                    uint8_t* src = source_symbols[lower_bound].data();
                    uint8_t* dst = decoder.symbol_data(lower_bound);

                    bool is_equal =
                        (std::memcmp(src, dst, max_symbol_bytes) == 0);

                    if (!is_equal)
                    {
                        std::cout
                            << "Mismatch symbol, lower_bound = " << lower_bound
                            << "\n";
                    }

                    ASSERT_TRUE(is_equal);
                }

                uint8_t* pop_symbol = decoder.pop_symbol();

                if (pop_symbol != nullptr)
                {
                    // Symbol was initialized
                    output_symbols.push_front(pop_symbol);
                }
            }

            // We should always have at least one output symbol ready
            ASSERT_TRUE(output_symbols.size() > 0);
        }

        ASSERT_TRUE(output_symbols.size() > 0);

        uint8_t* symbol = output_symbols.front();
        output_symbols.pop_front();

        // Uncoded or coded
        bool coded = rand() % 2;
        std::size_t random_index = 0;

        uint64_t seed = rand();
        auto window = encoder.stream_range();

        if (coded)
        {
            // Resize buffers
            coefficients.resize(generator.coefficients_bytes(window));

            generator.set_seed(seed);
            generator.generate(coefficients.data(), window);
            encoder.encode_symbol(symbol, window, coefficients.data());
        }
        else
        {
            std::uniform_int_distribution<> distribution(
                window.lower_bound(), window.upper_bound() - 1U);

            random_index = distribution(index_generator);

            ASSERT_GE(random_index, window.lower_bound());
            ASSERT_LT(random_index, window.upper_bound());

            encoder.encode_systematic_symbol(symbol, random_index);
        }

        // Update loop state
        ++iterations;
        control.advance();

        if (rand() % 2)
        {
            // Simulate 50% packet loss
            output_symbols.push_front(symbol);
            continue;
        }

        if (coded)
        {

            // Resize buffers
            coefficients.resize(generator.coefficients_bytes(window));

            generator.set_seed(seed);
            generator.generate(coefficients.data(), window);

            symbol = decoder.decode_symbol(symbol, max_symbol_bytes, window,
                                           coefficients.data());
        }
        else
        {
            symbol = decoder.decode_systematic_symbol(symbol, max_symbol_bytes,
                                                      random_index);
        }

        if (symbol != nullptr)
        {
            output_symbols.push_front(symbol);
        }
    }

    SCOPED_TRACE(testing::Message() << "decoded = " << decoded);
    SCOPED_TRACE(testing::Message() << "iterations = " << iterations);
}

// Test that everything works when sliding the window - also with losses.
// Since we only move the window and stream forward we may not decode all
// symbols.
TEST(test_slide, mix_coded_uncoded)
{
    mix_coded_uncoded(kodo::finite_field::binary);
    mix_coded_uncoded(kodo::finite_field::binary4);
    mix_coded_uncoded(kodo::finite_field::binary8);
}

void variable_size_symbols(kodo::finite_field field)
{
    SCOPED_TRACE(::testing::Message() << "field: " << kodo::to_string(field));
    auto seed = rand();
    SCOPED_TRACE(::testing::Message() << "seed: " << seed);
    srand(seed);

    // The number of symbols to process.
    std::size_t symbols = 10000;

    // This number determines at what point we give up on the decoding.
    // It will make sure we will not hang on bugs that cause infinite loops
    std::size_t max_iterations = (std::size_t)(symbols * 2.5);

    // The maximum number of symbols that can be encoded at a time.
    std::size_t max_window = 30;

    // The maximum number of bytes in a symbol
    std::size_t max_symbol_bytes = 25U;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

    decoder.enable_log(
        [](const auto& name, const auto& msg, void* data)
        {
            (void)data;
            std::cout << name << ": " << msg << "\n";
        },
        nullptr);
    decoder.disable_log();

    kodo::slide::generator::random_uniform generator(field);

    // Allocate our finite memory for encoding and decoding
    std::vector<std::vector<uint8_t>> encoder_storage;

    std::vector<uint8_t> coefficients;

    uint64_t iterations = 0;
    uint64_t systematic_index = 0;
    uint64_t symbols_decoded = 0;
    uint64_t symbols_lost = 0;
    while (symbols_decoded < symbols)
    {
        ++iterations;
        if (rand() % 10 > 4)
        {
            std::size_t random_size = 1 + (rand() % (max_symbol_bytes - 1));
            encoder_storage.emplace_back(std::vector<uint8_t>(random_size));
            auto& symbol = encoder_storage.back();

            std::memset(symbol.data(), encoder.stream_upper_bound(),
                        symbol.size());

            encoder.push_symbol(symbol.data(), symbol.size());
            if (encoder.stream_symbols() == max_window)
            {
                encoder.pop_symbol();
            }
        }

        if (encoder.stream_symbols() == 0)
        {
            continue;
        }

        bool systematic = systematic_index < encoder.stream_upper_bound();
        std::size_t index = systematic_index;
        auto window = encoder.stream_range();
        uint8_t* symbol = new uint8_t[encoder.max_symbol_bytes()];
        std::size_t size = 0;
        if (systematic)
        {
            size = encoder.encode_systematic_symbol(symbol, index);
            ASSERT_NE(0U, size);
            ++systematic_index;
        }
        else
        {
            uint64_t coefficient_seed = rand();

            coefficients.resize(generator.coefficients_bytes(window));
            generator.set_seed(coefficient_seed);
            generator.generate(coefficients.data(), window);

            size = encoder.encode_symbol(symbol, window, coefficients.data());
            ASSERT_NE(0U, size);
        }

        if (rand() % 100 < 5)
        {
            // Simulate 5% packet loss
            delete[] symbol;
            continue;
        }
        if (systematic)
        {
            while (!decoder.in_stream(index))
                decoder.push_symbol();

            symbol = decoder.decode_systematic_symbol(symbol, size, index);
        }
        else
        {
            while (!decoder.in_stream(window.upper_bound() - 1))
                decoder.push_symbol();
            symbol = decoder.decode_symbol(symbol, size, window,
                                           coefficients.data());
        }

        if (symbol != nullptr)
        {
            ASSERT_FALSE(systematic);
            // symbol linearly dependent
            delete[] symbol;
        }

        if (decoder.stream_symbols() > max_window)
        {
            auto index = decoder.stream_lower_bound();
            auto is_decoded = decoder.is_symbol_decoded(index);
            auto size_guess = decoder.symbol_bytes(index);
            symbol = decoder.pop_symbol();

            auto& expected_symbol = encoder_storage[index];
            EXPECT_GE(size_guess, expected_symbol.size());
            if (is_decoded)
            {
                EXPECT_EQ(expected_symbol,
                          std::vector<uint8_t>(symbol,
                                               symbol + expected_symbol.size()))
                    << index;
                symbols_decoded += 1;
            }
            else
            {
                ASSERT_FALSE(true)
                    << "index: " << index << " iteration " << iterations;
                symbols_lost += 1;
            }

            if (symbol != nullptr)
                delete[] symbol;
        }
        ASSERT_LT(iterations, max_iterations) << decoder.symbols_decoded();
    }
    EXPECT_EQ(0U, symbols_lost);

    decoder.reset(
        [](uint64_t index, const uint8_t* symbol, void* user_data)
        {
            (void)index;
            (void)user_data;
            delete[] symbol;
        });
}

// This tests exercises the encoder and decoder's support for variable
// symbol sizes
TEST(test_slide, variable_size_symbols)
{
    variable_size_symbols(kodo::finite_field::binary);
    variable_size_symbols(kodo::finite_field::binary4);
    variable_size_symbols(kodo::finite_field::binary8);
}

TEST(test_slide, symbols_of_decreasing_size)
{
    auto seed = rand();
    SCOPED_TRACE(::testing::Message() << "seed: " << seed);
    srand(seed);
    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(9);
    decoder.configure(9);

    decoder.enable_log(
        [](const auto& name, const auto& msg, void* data)
        {
            (void)data;
            std::cout << name << ": " << msg << "\n";
        },
        nullptr);
    decoder.disable_log();

    kodo::slide::generator::random_uniform generator(field);

    // Allocate our finite memory for encoding and decoding
    std::vector<std::vector<uint8_t>> encoder_storage;

    std::vector<uint8_t> coefficients;

    auto add_symbol = [&](std::size_t size)
    {
        encoder_storage.emplace_back(std::vector<uint8_t>(size));
        auto& symbol = encoder_storage.back();

        std::memset(symbol.data(), encoder.stream_upper_bound(), size);

        encoder.push_symbol(symbol.data(), symbol.size());
    };

    auto send_systematic = [&](std::size_t index)
    {
        auto size = encoder.symbol_bytes(index);
        ASSERT_NE(0U, size);
        auto symbol = new uint8_t[encoder.max_symbol_bytes()];
        encoder.encode_systematic_symbol(symbol, index);
        while (!decoder.in_stream(index))
            decoder.push_symbol();

        symbol = decoder.decode_systematic_symbol(symbol, size, index);

        if (symbol != nullptr)
        {
            // symbol linearly dependent
            delete[] symbol;
        }
    };

    auto send_coded = [&](auto window)
    {
        auto symbol = new uint8_t[encoder.max_symbol_bytes()];
        // Handle case where symbols were linearly dependent.
        auto max_interations = 3U;
        do
        {
            uint64_t coefficient_seed = rand();

            coefficients.resize(generator.coefficients_bytes(window));
            generator.set_seed(coefficient_seed);
            generator.generate(coefficients.data(), window);

            auto size =
                encoder.encode_symbol(symbol, window, coefficients.data());

            while (!decoder.in_stream(window.upper_bound() - 1))
                decoder.push_symbol();

            symbol = decoder.decode_symbol(symbol, size, window,
                                           coefficients.data());
            ASSERT_NE(0U, --max_interations);
        } while (symbol != nullptr);
    };

    // Add the following symbols to the encoder (with decreasing symbol sizes)
    add_symbol(9);
    add_symbol(8);
    add_symbol(7);
    add_symbol(6);
    add_symbol(5);
    add_symbol(4);
    add_symbol(3);
    add_symbol(2);
    add_symbol(1);

    // Construct this setup:
    // 0: [   1   0   0   0   0   0   0   0   0  ] 9 bytes
    // 1: none
    // 2: [   x   x   1   0   0   0   0   0   0  ] 7 bytes
    // 3: [   x   x   x   1   0   0   0   0   0  ] 6 bytes
    // 4: [   x   x   x   x   1   0   0   0   0  ] 5 bytes
    // 5: none
    // 6: [   x   x   x   x   x   x   1   0   0  ] 3 bytes
    // 7: [   x   x   x   x   x   x   x   1   0  ] 2 bytes
    // 8: [   x   x   x   x   x   x   x   x   1  ] 1 bytes

    send_systematic(0);
    // send_systematic(1); Lost
    send_systematic(2);
    send_systematic(3);
    send_systematic(4);
    // send_systematic(5); Lost
    send_systematic(6);
    send_systematic(7);
    send_systematic(8);

    EXPECT_TRUE(decoder.is_symbol_decoded(0));
    EXPECT_FALSE(decoder.is_symbol_decoded(1));
    EXPECT_FALSE(decoder.is_symbol_pivot(1));
    EXPECT_TRUE(decoder.is_symbol_decoded(2));
    EXPECT_TRUE(decoder.is_symbol_decoded(3));
    EXPECT_TRUE(decoder.is_symbol_decoded(4));
    EXPECT_FALSE(decoder.is_symbol_decoded(5));
    EXPECT_FALSE(decoder.is_symbol_pivot(5));
    EXPECT_TRUE(decoder.is_symbol_decoded(6));
    EXPECT_TRUE(decoder.is_symbol_decoded(7));
    EXPECT_TRUE(decoder.is_symbol_decoded(8));

    // Construct this setup:
    // 0: [   1   0   0   0   0   0   0   0   0  ] 9 bytes
    // 1: [   0   1   0   0   0 125   0   0   0  ] 9 bytes (CODED)
    // 2: [   x   x   1   0   0   0   0   0   0  ] 7 bytes
    // 3: [   x   x   x   1   0   0   0   0   0  ] 6 bytes
    // 4: [   x   x   x   x   1   0   0   0   0  ] 5 bytes
    // 5: none
    // 6: [   x   x   x   x   x   x   1   0   0  ] 3 bytes
    // 7: [   x   x   x   x   x   x   x   1   0  ] 2 bytes
    // 8: [   x   x   x   x   x   x   x   x   1  ] 1 bytes

    send_coded(encoder.stream_range());

    EXPECT_FALSE(decoder.is_symbol_decoded(1));
    EXPECT_TRUE(decoder.is_symbol_pivot(1));
    EXPECT_EQ(9U, decoder.symbol_bytes(1));

    // Pop the first symbol in the encoder so that the symbol pivot at index 1
    // is larger than the produced repair after the pop.
    encoder.pop_symbol();

    send_coded(encoder.stream_range());

    // This will swap the incoming symbol with symbol 1 and then insert the
    // symbol initially at pivot 1 at index 5, leaving the decoder fully
    // decoded.
    EXPECT_TRUE(decoder.is_symbol_decoded(1));
    EXPECT_TRUE(decoder.is_symbol_decoded(5));
    EXPECT_EQ(decoder.symbols_decoded(), decoder.stream_symbols());

    decoder.reset([](uint64_t, const uint8_t* symbol, void*)
                  { delete[] symbol; });
}

TEST(test_slide, late_join)
{
    auto seed = rand();
    SCOPED_TRACE(::testing::Message() << "seed: " << seed);
    srand(seed);

    // Set the capacity of the decoder (this is the number of encoded
    // symbols that are used in the decoding process).
    std::size_t symbols = 500;

    // This number determines at what point we give up on the decoding.
    // It will make sure we will not hang on bugs that cause infinite loops
    std::size_t max_iterations = (std::size_t)(symbols * 2.5);

    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;

    // The size of a symbol in bytes
    std::size_t max_symbol_bytes = 200U;

    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

    kodo::slide::generator::random_uniform generator(field);

    // Allocate our finite memory for encoding and decoding
    std::vector<std::vector<uint8_t>> encoder_storage;
    std::vector<std::vector<uint8_t>> decoder_storage;

    encoder_storage.resize(symbols);
    decoder_storage.resize(symbols);

    // Symbol size generator
    std::mt19937 size_generator(rand());
    std::uniform_int_distribution<> size_distribution(1, max_symbol_bytes);

    for (auto& symbol : encoder_storage)
    {
        // The encoder sees a stream of variable symbols
        symbol.resize(size_distribution(size_generator));
        ASSERT_TRUE(symbol.size() > 0U);
        ASSERT_TRUE(symbol.size() <= max_symbol_bytes);
        std::generate(symbol.begin(), symbol.end(), rand);
    }

    /// Simulate the the encoder has been running for a while.
    while (encoder.stream_lower_bound() < symbols)
    {
        std::vector<uint8_t> buffer(size_distribution(size_generator));
        encoder.push_symbol(buffer.data(), buffer.size());
        encoder.pop_symbol();
    }

    EXPECT_EQ(symbols, encoder.stream_lower_bound());
    EXPECT_EQ(symbols, encoder.stream_upper_bound());
    EXPECT_EQ(0U, encoder.stream_symbols());
    EXPECT_EQ(0U, decoder.stream_lower_bound());
    EXPECT_TRUE(decoder.is_stream_empty());

    decoder.set_stream_lower_bound(symbols);

    for (auto& _ : decoder_storage)
    {
        (void)_;
        decoder.push_symbol();
    }

    std::vector<uint8_t> coefficients;
    std::size_t iterations = 0;
    while (decoder.symbols_decoded() < symbols)
    {
        if (encoder.stream_symbols() < symbols && rand() % 2)
        {
            std::size_t index = encoder.stream_symbols();
            const uint8_t* data = encoder_storage.at(index).data();
            std::size_t size = encoder_storage.at(index).size();

            encoder.push_symbol(data, size);
        }

        if (encoder.stream_symbols() == 0)
        {
            continue;
        }

        auto window = encoder.stream_range();
        coefficients.resize(generator.coefficients_bytes(window));
        uint64_t coefficient_seed = rand();
        generator.set_seed(coefficient_seed);
        generator.generate(coefficients.data(), window);

        // The rank will always be incremented when a new buffer is needed.
        auto& symbol = decoder_storage[decoder.rank()];
        symbol.resize(encoder.max_symbol_bytes());
        auto size =
            encoder.encode_symbol(symbol.data(), window, coefficients.data());
        ASSERT_NE(0U, size);

        decoder.decode_symbol(symbol.data(), size, window, coefficients.data());

        ++iterations;
        ASSERT_LT(iterations, max_iterations);
    }

    // Resize the output of the decoder to match the actual encoded symbols
    for (std::size_t i = 0; i < symbols; ++i)
    {
        SCOPED_TRACE(::testing::Message() << "Symbol index: " << i);

        const uint8_t* src = encoder_storage[i].data();
        const uint8_t* dst = decoder.symbol_data(symbols + i);
        std::size_t size = encoder_storage[i].size();

        EXPECT_GE(decoder.symbol_bytes(symbols + i), size);

        bool is_equal = (std::memcmp(src, dst, size) == 0);

        if (!is_equal)
        {
            std::cout << "Mismatch symbol = " << i << " size " << size
                      << std::endl;
        }

        ASSERT_TRUE(is_equal);
    }
}

TEST(test_slide, reorder)
{
    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;
    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);
    kodo::slide::generator::random_uniform generator(field);

    encoder.configure(100);
    decoder.configure(100);

    std::vector<std::vector<uint8_t>> encoder_storage = {
        {1, 1},
        {2, 2},
    };

    encoder.push_symbol(encoder_storage[0].data(), encoder_storage[0].size());
    encoder.push_symbol(encoder_storage[1].data(), encoder_storage[1].size());

    auto range = encoder.stream_range();

    std::vector<uint8_t> coefficients(generator.coefficients_bytes(range));
    generator.generate(coefficients.data(), range);
    EXPECT_NE(0U, coefficients[0]);
    EXPECT_NE(0U, coefficients[1]);

    // Send the encoded symbol first
    std::vector<uint8_t> buffer1(encoder.max_symbol_bytes());
    auto size =
        encoder.encode_symbol(buffer1.data(), range, coefficients.data());
    decoder.push_symbol();
    decoder.push_symbol();
    EXPECT_EQ(0U, decoder.rank());
    decoder.decode_symbol(buffer1.data(), size, range, coefficients.data());
    // This will increase the rank but not produce any decoded symbols
    EXPECT_EQ(1U, decoder.rank());
    EXPECT_FALSE(decoder.is_symbol_decoded(0));
    EXPECT_FALSE(decoder.is_symbol_decoded(1));

    // Now send the second symbol as a systematic one.
    std::vector<uint8_t> buffer2(encoder.max_symbol_bytes());
    size = encoder.encode_systematic_symbol(buffer2.data(), 1);
    decoder.decode_systematic_symbol(buffer2.data(), size, 1);
    // This will increase the rank to 2
    EXPECT_EQ(2U, decoder.rank());

    // And we can now expect both symbols to be decoded
    EXPECT_TRUE(decoder.is_symbol_decoded(0));
    EXPECT_TRUE(decoder.is_symbol_decoded(1));
}
