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
#include <iostream>
#include <vector>

#include <kodo/block/decoder.hpp>
#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>

int main()
{
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    std::size_t symbols = 16;
    std::size_t symbol_size = 1400;
    kodo::finite_field field = kodo::finite_field::binary8;

    //! [0]
    // Create an encoder, decoder1, decoder2, and generator using the specified
    // parameters
    kodo::block::encoder encoder{field};
    kodo::block::decoder decoder1{field};
    kodo::block::decoder decoder2{field};
    kodo::block::generator::random_uniform generator{field};

    encoder.configure(symbols, symbol_size);
    decoder1.configure(symbols, symbol_size);
    decoder2.configure(symbols, symbol_size);
    generator.configure(symbols);
    //! [1]

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
    std::vector<uint8_t> recode_coefficients(
        generator.max_coefficients_bytes());
    std::vector<uint8_t> data_in(encoder.block_bytes());

    // Just for fun - fill the data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder.set_symbols_storage(data_in.data());

    // Define data buffers where the symbols should be decoded
    std::vector<uint8_t> data_out1(decoder1.block_bytes());
    std::vector<uint8_t> data_out2(decoder2.block_bytes());
    decoder1.set_symbols_storage(data_out1.data());
    decoder2.set_symbols_storage(data_out2.data());

    uint32_t encoded_count = 0;
    uint32_t dropped_count = 0;

    //! [2]
    while (!decoder2.is_complete())
    {
        // Generate coefficients
        generator.generate(coefficients.data());
        // Encode a packet into the symbol buffer
        encoder.encode_symbol(symbol.data(), coefficients.data());

        ++encoded_count;

        if (rand() % 2)
        {
            ++dropped_count;
        }
        else
        {
            // Pass that packet to the decoder1
            decoder1.decode_symbol(symbol.data(), coefficients.data());
        }

        // Generate coefficients
        generator.generate_recode(coefficients.data(), decoder1);
        // Recode a packet into the symbol buffer
        decoder1.recode_symbol(symbol.data(), recode_coefficients.data(),
                               coefficients.data());

        if (rand() % 2)
        {
            ++dropped_count;
        }
        else
        {
            // Pass the recoded packet to decoder two
            decoder2.decode_symbol(symbol.data(), recode_coefficients.data());
        }
    }
    //! [3]
    std::cout << "Encoded count = " << encoded_count << std::endl;
    std::cout << "Dropped count = " << dropped_count << std::endl;

    if (data_out1 == data_in && data_out2 == data_in)
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
    return 0;
}
