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
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

#include <kodo/perpetual/decoder.hpp>
#include <kodo/perpetual/encoder.hpp>
#include <kodo/perpetual/generator/random_uniform.hpp>
#include <kodo/perpetual/offset/random_uniform.hpp>

/// @example perpetual/encode_decode_simple.cpp
///
/// Simple example showing how to encode and decode a block of memory using
/// perpetual coders

int main()
{
    // Seed the random number generator to get different random data
    srand(static_cast<uint32_t>(time(0)));

    // Create an encoder and decoder.
    kodo::perpetual::encoder encoder(kodo::perpetual::width::_32);
    kodo::perpetual::decoder decoder(kodo::perpetual::width::_32);

    // Pick the number of bytes to encode/decode.
    auto block_bytes = 2000000;

    // Pick the size of each symbol in bytes
    auto symbol_bytes = 100;

    // Configure the coders, the encoder and decoder must be configured
    // identically to be compatible.
    encoder.configure(block_bytes, symbol_bytes);
    decoder.configure(block_bytes, symbol_bytes);

    // Create generator and offset generator.
    kodo::perpetual::generator::random_uniform generator(
        kodo::perpetual::width::_32);

    kodo::perpetual::offset::random_uniform offset_generator;

    // Configure generator and offset generator.
    // The generators must similarly be configured based on the encoder/decoder.
    offset_generator.configure(encoder.symbols());

    // Allocate some storage for a symbol.
    std::vector<uint8_t> symbol(encoder.symbol_bytes());

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

    std::size_t symbols_sent = 0;
    std::size_t symbols_received = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_total =
        std::chrono::high_resolution_clock::now();

    std::chrono::time_point<std::chrono::high_resolution_clock> start_interval =
        std::chrono::high_resolution_clock::now();

    std::chrono::milliseconds interval;
    std::chrono::seconds total;

    while (!decoder.is_complete())
    {
        if ((symbols_received % 10000) == 0)
        {
            auto end = std::chrono::high_resolution_clock::now();

            interval = std::chrono::duration_cast<std::chrono::milliseconds>(
                end - start_interval);

            total = std::chrono::duration_cast<std::chrono::seconds>(
                end - start_total);

            start_interval = std::chrono::high_resolution_clock::now();
        }

        // Generate the offset
        auto offset = offset_generator.offset();

        // Generate the coefficients into the symbol buffer
        auto coefficients = generator.generate(symbols_sent);

        // Encode a symbol into the symbol buffer
        encoder.encode_symbol(symbol.data(), coefficients, offset);

        ++symbols_sent;

        if (rand() % 2)
        {
            if (rand() % 2)
            {
                continue;
            }
        }

        // Note; if this were two septate applications communicating over a
        // network the encoder and decoder would need to exchange some
        // information.
        // Besides the obvious symbol data, the encoder would also need to
        // share the coefficients and offset
        // (the state for the decoder to generate the information on it's own).

        // Pass the symbol, coefficients, and offset to the decoder
        decoder.decode_symbol(symbol.data(), coefficients, offset);

        ++symbols_received;

        std::cout << "rank = " << decoder.rank()
                  << ", needed = " << symbols_received
                  << ", linear dept. = " << symbols_received - decoder.rank()
                  << ", elapsed_interval = " << interval.count()
                  << ", elapsed_total = " << total.count() << std::endl;
    }

    // Check if we properly decoded the data
    if (data_out == data_in)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}