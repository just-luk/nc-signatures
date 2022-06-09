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

#include <kodo/slide/encoder.hpp>

#include <kodo/slide/generator/random_uniform.hpp>

#include <numeric>

#include <gtest/gtest.h>

TEST(slide_test_encoder, field)
{
    kodo::slide::encoder encoder;

    {
        encoder = kodo::slide::encoder(kodo::finite_field::binary);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary);
    }
    {
        encoder = kodo::slide::encoder(kodo::finite_field::binary4);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary4);
    }
    {
        encoder = kodo::slide::encoder(kodo::finite_field::binary8);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary8);
    }
    {
        encoder = kodo::slide::encoder(kodo::finite_field::binary16);
        EXPECT_EQ(encoder.field(), kodo::finite_field::binary16);
    }
}

TEST(slide_test_encoder, configure)
{
    kodo::slide::encoder encoder{kodo::finite_field::binary8};
    std::size_t max_symbol_bytes = 42;
    encoder.configure(max_symbol_bytes);
    EXPECT_TRUE(encoder.is_stream_empty());
    EXPECT_EQ(max_symbol_bytes, encoder.max_symbol_bytes());
}

TEST(slide_test_encoder, basic_api)
{
    kodo::finite_field field = kodo::finite_field::binary8;
    std::size_t max_symbol_bytes = 200U;

    kodo::slide::generator::random_uniform generator(field);

    kodo::slide::encoder encoder(field);
    encoder.configure(max_symbol_bytes);

    EXPECT_TRUE(encoder.is_stream_empty());

    std::vector<uint8_t> coefficients;
    std::vector<uint8_t> symbol(max_symbol_bytes);

    while (encoder.stream_lower_bound() < 100)
    {
        encoder.push_symbol(symbol.data(), symbol.size());
        EXPECT_FALSE(encoder.is_stream_empty());

        generator.set_seed(rand());

        auto window = encoder.stream_range();
        coefficients.resize(generator.coefficients_bytes(window));
        generator.generate(coefficients.data(), window);

        // The coefficient vector must not be all zero. To check this we simply
        // sum the values in the vector and see that it is > 0.
        uint32_t coefficients_sum =
            std::accumulate(coefficients.begin(), coefficients.end(), 0U);

        EXPECT_NE(0U, coefficients_sum);

        EXPECT_NE(0U, encoder.encode_systematic_symbol(symbol.data(),
                                                       window.lower_bound()));
        EXPECT_NE(0U, encoder.encode_symbol(symbol.data(), window,
                                            coefficients.data()));

        encoder.pop_symbol();
    }
}
