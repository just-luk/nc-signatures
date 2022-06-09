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

#include <kodo/detail/perpetual/inner_generator.hpp>
#include <kodo/detail/perpetual/offset_generator.hpp>
#include <kodo/detail/perpetual/stack_decoder.hpp>
#include <kodo/detail/perpetual/stack_encoder.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

namespace
{
template <class InnerCoefficientsType>
void test_stacks(uint64_t seed)
{
    SCOPED_TRACE(testing::Message() << "seed = " << seed);
    srand(seed);
    byte_count block_bytes{1000000};
    byte_count symbol_bytes{140};
    symbol_count outer_interval{8};
    std::size_t outer_segments{8};
    double mapping_threshold = 0.98;

    typename perpetual::stack_decoder<InnerCoefficientsType>::config
        decoder_config;
    decoder_config.symbol_bytes = symbol_bytes;
    decoder_config.block_bytes = block_bytes;
    decoder_config.outer_interval = outer_interval;
    decoder_config.outer_segments = outer_segments;
    decoder_config.mapping_threshold = mapping_threshold;

    typename perpetual::stack_encoder<InnerCoefficientsType>::config
        encoder_config;
    encoder_config.symbol_bytes = symbol_bytes;
    encoder_config.block_bytes = block_bytes;
    encoder_config.outer_interval = outer_interval;
    encoder_config.outer_segments = outer_segments;

    perpetual::stack_decoder<InnerCoefficientsType> decoder;
    decoder.configure(decoder_config);

    perpetual::stack_encoder<InnerCoefficientsType> encoder;
    encoder.configure(encoder_config);

    EXPECT_EQ(encoder.symbols(), decoder.symbols());
    EXPECT_EQ(encoder.outer_symbols(), decoder.outer_symbols());
    EXPECT_EQ(encoder.outer_interval(), decoder.outer_interval());
    EXPECT_EQ(encoder.outer_segments(), decoder.outer_segments());
    EXPECT_EQ(encoder.block_bytes(), decoder.block_bytes());

    SCOPED_TRACE(testing::Message()
                 << "has_partial_symbol = " << decoder.has_partial_symbol());
    SCOPED_TRACE(testing::Message() << "symbols = " << decoder.symbols().value);
    SCOPED_TRACE(testing::Message()
                 << "block bytes = " << decoder.block_bytes().value);
    SCOPED_TRACE(testing::Message()
                 << "outer symbols = " << decoder.outer_symbols().value);
    SCOPED_TRACE(testing::Message()
                 << "data symbols = " << decoder.data_symbols().value);

    // Generate some input data
    std::vector<uint8_t> data_in(block_bytes.value);
    std::vector<uint8_t> data_out(block_bytes.value);

    std::generate(data_in.begin(), data_in.end(), rand);
    encoder.set_symbols_storage(data_in.data());
    decoder.set_symbols_storage(data_out.data());

    // decoder.enable_log(
    //     [](auto name, auto message, auto data) {
    //         (void)data;
    //         std::cout << name << ": " << message << std::endl;
    //     },
    //     nullptr);

    // Buffer for an encoded symbol
    std::vector<uint8_t> symbol(symbol_bytes.value);

    // The coefficient generator used for the inner code
    perpetual::inner_generator<InnerCoefficientsType> generator;

    // The offset generator
    perpetual::offset_generator offset_generator;
    perpetual::offset_generator::config offset_generator_config;
    offset_generator_config.symbols = encoder.symbols();
    offset_generator.configure(offset_generator_config);
    offset_generator.set_seed(seed);

    // The seed for the generated coefficients - should increment once
    // per packet we send
    uint32_t coefficients_seed = 0;

    // We may be picking the same offset twice but the inner coding
    // coefficients are different which is fine. Probably the issue is that
    // we have limited space in the outer code and we use the offset as a
    // way way to map to the outer symbol storage - however that is not really
    // reliable since we can pick the same offset twice - and still use
    // different inner coding coefficients. This would probably be solved
    // by first passing symbols via the inner code as intended - so we
    // probably just need to do that...

    auto max_iterations = decoder.data_symbols().value * 5;
    while (!decoder.is_complete())
    {
        symbol_index offset = offset_generator.offset();

        InnerCoefficientsType coefficients =
            generator.generate(coefficients_seed);
        coefficients_seed += 1;

        encoder.encode_symbol(symbol.data(), coefficients, offset);
        decoder.decode_symbol(symbol.data(), coefficients, offset);
        max_iterations--;

        SCOPED_TRACE(testing::Message()
                     << "outer_rank: " << decoder.outer_rank());
        SCOPED_TRACE(testing::Message()
                     << "inner_rank: " << decoder.inner_rank());
        SCOPED_TRACE(testing::Message()
                     << "inner_data_rank: " << decoder.inner_data_rank());
        SCOPED_TRACE(testing::Message()
                     << "inner_outer_rank: " << decoder.inner_outer_rank());
        ASSERT_NE(0U, max_iterations);
    }

    EXPECT_EQ(data_in.size(), data_out.size());
    EXPECT_EQ(data_in, data_out);
}
}

TEST(detail_perpetual_test_stacks, coding)
{
    test_stacks<uint64_t>(rand());
    test_stacks<uint32_t>(rand());
    test_stacks<uint16_t>(rand());
    test_stacks<uint8_t>(rand());
}
