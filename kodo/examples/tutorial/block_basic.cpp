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
    //! [0]
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    std::size_t symbols = 16;
    std::size_t symbol_size = 1400;
    kodo::finite_field field = kodo::finite_field::binary8;
    //! [1]
    // Create an encoder, decoder and generator and configure them using the
    // specified parameters
    kodo::block::encoder encoder{field};
    kodo::block::decoder decoder{field};
    kodo::block::generator::random_uniform generator{field};
    encoder.configure(symbols, symbol_size);
    decoder.configure(symbols, symbol_size);
    generator.configure(symbols);
    //! [2]
    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
    std::vector<uint8_t> data_in(encoder.block_bytes());

    // Just for fun - fill the data with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder.set_symbols_storage(data_in.data());

    // Define a data buffer where the symbols should be decoded
    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());
    //! [3]
    uint32_t encoded_count = 0;

    while (!decoder.is_complete())
    {
        // Generate coefficients
        generator.generate(coefficients.data());
        // Encode a packet into the symbol buffer
        encoder.encode_symbol(symbol.data(), coefficients.data());

        ++encoded_count;

        // Pass that packet to the decoder
        decoder.decode_symbol(symbol.data(), coefficients.data());
    }

    std::cout << "Encoded count = " << encoded_count << std::endl;
    //! [4]
    // Check the decoded data
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
    //! [5]
}
