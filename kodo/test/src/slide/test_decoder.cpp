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

#include <kodo/slide/decoder.hpp>

#include <kodo/slide/generator/random_uniform.hpp>

#include <numeric>

#include <gtest/gtest.h>

TEST(slide_test_decoder, field)
{
    kodo::slide::decoder decoder;

    {
        decoder = kodo::slide::decoder(kodo::finite_field::binary);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary);
    }
    {
        decoder = kodo::slide::decoder(kodo::finite_field::binary4);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary4);
    }
    {
        decoder = kodo::slide::decoder(kodo::finite_field::binary8);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary8);
    }
    {
        decoder = kodo::slide::decoder(kodo::finite_field::binary16);
        EXPECT_EQ(decoder.field(), kodo::finite_field::binary16);
    }
}

TEST(slide_test_decoder, configure)
{
    kodo::slide::decoder decoder{kodo::finite_field::binary8};
    std::size_t max_symbol_bytes = 42;
    decoder.configure(max_symbol_bytes);
    EXPECT_TRUE(decoder.is_stream_empty());
    EXPECT_EQ(max_symbol_bytes, decoder.max_symbol_bytes());
}

TEST(slide_test_decoder, basic_api)
{
    kodo::finite_field field = kodo::finite_field::binary8;
    std::size_t max_symbol_bytes = 200U;

    kodo::slide::generator::random_uniform generator(field);

    kodo::slide::decoder decoder(field);
    decoder.configure(max_symbol_bytes);

    std::vector<uint8_t> coefficients;
    std::vector<uint8_t> input_symbol(max_symbol_bytes);
    std::vector<std::vector<uint8_t>> output_symbols;
    std::size_t max_stream_symbols = 20;

    std::set<uint64_t> decoded_symbols;

    output_symbols.reserve(max_stream_symbols);

    auto decoded_callback = [](uint64_t index, void* data)
    {
        std::set<uint64_t>* set = static_cast<std::set<uint64_t>*>(data);
        set->insert(index);
    };

    std::size_t symbols_pivot = 0;

    auto pivot_callback = [](uint64_t index, void* data)
    {
        (void)index;
        std::size_t* symbols_pivot = static_cast<std::size_t*>(data);
        (*symbols_pivot)++;
    };

    decoder.on_symbol_decoded(decoded_callback, &decoded_symbols);

    decoder.on_symbol_pivot(pivot_callback, &symbols_pivot);

    auto i = 0;

    while (decoder.stream_upper_bound() < 100)
    {
        uint8_t* symbol = nullptr;

        if (decoder.stream_symbols() < max_stream_symbols)
        {
            output_symbols.emplace_back(max_symbol_bytes);
            symbol = output_symbols.back().data();
            decoder.push_symbol();
        }
        else
        {
            // We have reached the maximum number of symbols - so we start to
            // pop and push
            symbol = decoder.pop_symbol();
            decoder.push_symbol();
        }

        EXPECT_TRUE(decoder.stream_symbols() > 0U);
        EXPECT_TRUE(decoder.stream_symbols() <= 20U);

        kodo::slide::range window{decoder.stream_lower_bound(),
                                  decoder.stream_upper_bound()};

        generator.set_seed(rand());

        coefficients.resize(generator.coefficients_bytes(window));

        do
        {

            ASSERT_NE(nullptr, symbol);
            generator.generate(coefficients.data(), window);
            symbol = decoder.decode_symbol(symbol, max_symbol_bytes, window,
                                           coefficients.data());
            if (symbol == nullptr)
            {
                EXPECT_TRUE(decoder.is_symbol_pivot(i));
                i += 1;
                EXPECT_EQ(symbols_pivot, i);
            }

            // We keep trying to decode until it works
        } while (symbol != nullptr);
    }

    while (!decoder.is_stream_empty())
    {
        decoder.pop_symbol();
    }

    EXPECT_TRUE(decoder.stream_symbols() == 0U);
    EXPECT_TRUE(decoder.is_stream_empty());
    EXPECT_EQ(decoder.stream_lower_bound(), 100U);
    EXPECT_EQ(decoder.stream_upper_bound(), 100U);

    decoder.set_stream_lower_bound(200U);

    EXPECT_TRUE(decoder.stream_symbols() == 0U);
    EXPECT_TRUE(decoder.is_stream_empty());
    EXPECT_EQ(decoder.stream_lower_bound(), 200U);
    EXPECT_EQ(decoder.stream_upper_bound(), 200U);

    output_symbols.resize(0);

    while (decoder.stream_upper_bound() < 300)
    {
        uint8_t* symbol = nullptr;

        if (decoder.stream_symbols() < max_stream_symbols)
        {
            output_symbols.emplace_back(max_symbol_bytes);
            symbol = output_symbols.back().data();
            decoder.push_symbol();
        }
        else
        {
            // We have reached the maximum number of symbols - so we start to
            // pop and push
            symbol = decoder.pop_symbol();
            decoder.push_symbol();
        }

        EXPECT_TRUE(decoder.stream_symbols() > 0U);
        EXPECT_TRUE(decoder.stream_symbols() <= 20U);

        kodo::slide::range window{decoder.stream_lower_bound(),
                                  decoder.stream_upper_bound()};

        generator.set_seed(rand());

        coefficients.resize(generator.coefficients_bytes(window));

        do
        {
            generator.generate(coefficients.data(), window);

            symbol = decoder.decode_symbol(symbol, max_symbol_bytes, window,
                                           coefficients.data());

        } while (symbol != nullptr);
    }

    // We should see at least 200 symbols decoded
    EXPECT_EQ(decoded_symbols.size(), 200);
    EXPECT_EQ(symbols_pivot, 200);
}
