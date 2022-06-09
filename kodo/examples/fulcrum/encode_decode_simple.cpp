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
#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

#include <kodo/finite_field.hpp>

#include <kodo/fulcrum/decoder.hpp>
#include <kodo/fulcrum/encoder.hpp>
#include <kodo/fulcrum/generator/random_uniform.hpp>

/// @example fulcrum/encode_decode_simple.cpp
///
/// Simple example showing how to encode and decode a block
/// of memory using a fulcrum RLNC code.
int main()
{
    // Seed the random number generator to get different random data
    srand(static_cast<uint32_t>(time(0)));

    // Pick the finite field to use for the encoding and decoding.
    auto field = kodo::finite_field::binary8;

    // Create an encoder and decoder.
    kodo::fulcrum::encoder encoder(field);
    kodo::fulcrum::decoder decoder(field);

    // Pick the number of symbols to encode/decode.
    auto symbols = 150;

    // Pick the number of symbols for the expansion.
    auto expansion = 15;

    // Pick the size of each symbol in bytes
    auto symbol_bytes = 1400;

    // Configure the coders, the encoder and decoder must be configured
    // identically to be compatible.
    encoder.configure(symbols, symbol_bytes, expansion);
    decoder.configure(symbols, symbol_bytes, expansion);

    // Create generator generator.
    kodo::fulcrum::generator::random_uniform generator;

    // The generator must similarly be configured based on the encoder/decoder.
    generator.configure(encoder.symbols(), encoder.expansion());

    // Allocate some storage for a symbol.
    std::vector<uint8_t> symbol(encoder.symbol_bytes());

    // Allocate some storage for the coefficients.
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Allocate some data to encode. In this case we make a buffer
    // with the same size as the encoder's block size (the max.
    // amount a single encoder can encode)
    std::vector<uint8_t> data_in(encoder.block_bytes());

    // Just for fun - fill data_in with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder.set_symbols_storage(data_in.data());

    // Define a data buffer where the symbols should be decoded
    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    // Keep track of the systematic symbols
    std::size_t systematic_index = 0;

    // Lose packets with 10% probability
    auto loss_propability = 20;

    decoder.on_symbol_decoded(
        [](auto index, auto user_data)
        {
            (void)user_data;
            std::cout << " " << index;
        },
        nullptr);

    // while (!decoder.is_complete())
    for (std::size_t i = 0; i < symbols * 2U; i++)
    {
        if (decoder.is_complete())
        {
            break;
        }

        if (encoder.inner_symbols() > systematic_index)
        {
            std::cout << "systematic symbol";
            if (encoder.rank() <= systematic_index)
                std::cout << " (expansion)";
            auto index = systematic_index;
            systematic_index += 1;
            encoder.encode_systematic_symbol(symbol.data(), index);

            // Drop packet based on loss probability
            if (rand() % 100 < loss_propability)
            {
                std::cout << " - lost" << std::endl;
                continue;
            }

            decoder.decode_systematic_symbol(symbol.data(), index);
            std::cout << " - decoded, rank now " << decoder.rank() << std::endl;
        }
        else
        {
            // The RLNC code is rateless, which means, in theory, that it
            // can generate an infinite number of coded packets.
            std::cout << "coded symbol";

            // Draw a seed
            auto seed = rand();
            generator.set_seed(seed);

            // Generate the coefficients into the symbol buffer
            generator.generate(coefficients.data());

            // Encode a symbol into the symbol buffer
            encoder.encode_symbol(symbol.data(), coefficients.data());

            // Drop packet based on loss probability
            if (rand() % 100 < loss_propability)
            {
                std::cout << " - lost" << std::endl;
                continue;
            }

            // In this example the coefficients are available, but in
            // certain use cases - especially network based it might not be
            // feasible to transmit the coefficients. In this case the seed
            // can be used for generating the coefficients again.
            generator.set_seed(seed);
            generator.generate(coefficients.data());
            decoder.decode_symbol(symbol.data(), coefficients.data());
            std::cout << " - decoded, rank now " << decoder.rank() << std::endl;
        }
    }

    // Check if we properly decoded the data
    if (data_out == data_in)
    {
        std::cout << "Data decoded correctly" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
        return 1;
    }
}
