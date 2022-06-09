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
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

#include <kodo/finite_field.hpp>

#include <kodo/block/decoder.hpp>
#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>

// Callback to call on decode_symbol()
void decoding_callback(uint64_t index, void* user_data)
{
    (void)user_data;
    std::cout << "  Symbol " << index << " Decoded" << std::endl;
}

/// @example rlnc/on_the_fly_repair.cpp
///
/// Simple example showing how to encode and decode a block
/// of memory using an RLNC code.
int main()
{
    // struct for a packet_header to hold the information used in the decoding
    struct packet_header
    {
        bool is_systematic{false};
        uint64_t seed{0};
        uint64_t systematic_index{0};
        uint64_t rank{0};
    };

    // Seed the random number generator to get different random data
    srand(static_cast<uint32_t>(time(0)));

    // Pick the finite field to use for the encoding and decoding.
    auto field = kodo::finite_field::binary8;

    // Create an encoder and decoder.
    kodo::block::encoder encoder(field);
    kodo::block::decoder decoder(field);

    // Pick the number of symbols to encode/decode.
    std::size_t symbols = 150;

    // Pick the size of each symbol in bytes
    std::size_t symbol_bytes = 1400;

    // Configure the coders, the encoder and decoder must be configured
    // identically to be compatible.
    encoder.configure(symbols, symbol_bytes);
    decoder.configure(symbols, symbol_bytes);

    // Set the decoding callback.
    decoder.on_symbol_decoded(decoding_callback, nullptr);

    // Create coefficients generator.
    kodo::block::generator::random_uniform generator(field);

    // The generator must similarly be configured based on the encoder/decoder.
    generator.configure(encoder.symbols());

    // Allocate some storage for a symbol.
    std::vector<uint8_t> data_in(encoder.block_bytes());

    // Just for fun - fill the data_in with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Allocate some storage for a symbol.
    std::vector<uint8_t> symbol(encoder.symbol_bytes());

    // Allocate some storage for the coefficients.
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Keep track of the systematic symbols
    std::size_t systematic_index = 0;

    // Lose packets with 50% probability
    std::size_t loss_propability = 10;

    // Define a data buffer where the symbols should be decoded
    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    // Run until we have enough symbols to decode the complete data
    while (!decoder.is_complete())
    {
        // Initialize a header for a new packet.
        packet_header header;

        // If we still have some systematic symbols to generate we will try to
        // generate some. The other variable is to simulate packet arrivals.
        // Now? true/false.
        if ((encoder.rank() < symbols) && (rand() % 5))
        {
            // We add a systematic symbol to the encoder. The encoder makes a
            // shallow copy, so we work on the data_in pointer instead of
            // generating a symbol each time.
            encoder.set_symbol_storage(
                data_in.data() + encoder.rank() * symbol_bytes, encoder.rank());
        }
        // This handles the special case where the first if-statement fails in
        // the first iteration.
        if (encoder.rank() == 0)
        {
            continue;
        }

        // If we have fixed all packet loss until now, the rank would be greater
        // than the systematic_index. Therefore we can generate a systematic
        // symbol.
        if (encoder.rank() > systematic_index)
        {
            std::cout << "Systematic symbol: ";
            encoder.encode_systematic_symbol(symbol.data(), systematic_index);

            // The decoder needs to know the systematic index to place
            // the symbol correctly in the data_out buffer
            header.is_systematic = true;
            header.systematic_index = systematic_index;
            ++systematic_index;
        }
        // If we haven't fixed all packet loss until now, we will generate a
        // coded symbol.
        else
        {
            std::cout << "Coded symbol:      ";

            // Draw a seed
            auto seed = rand();
            // Seed the coefficients generator
            generator.set_seed(seed);

            // Generate the coefficients into the symbol buffer
            generator.generate_partial(coefficients.data(), encoder.rank());

            // Encode a symbol into the symbol buffer
            encoder.encode_symbol(symbol.data(), coefficients.data());

            // The decoder needs to know the seed and rank to decode the coded
            // symbol. The rank is needed for the generator.generate_partial()
            // function.
            header.is_systematic = false;
            header.seed = seed;
            header.rank = encoder.rank();
        }
        // Simulate a packet loss.
        if (rand() % 100U < loss_propability)
        {
            std::cout << "lost" << std::endl;
            continue;
        }
        std::cout << "received" << std::endl;
        // If the sent packet was systematic, we handle it so.
        if (header.is_systematic)
        {
            decoder.decode_systematic_symbol(symbol.data(),
                                             header.systematic_index);
        }
        // Otherwise, we handle it as a coded symbol.
        else
        {
            // Seed the generator
            generator.set_seed(header.seed);

            // Generate the coefficients into the symbol buffer
            generator.generate_partial(coefficients.data(), header.rank);

            // Encode a symbol into the symbol buffer
            decoder.decode_symbol(symbol.data(), coefficients.data());
        }
        std::cout << "  Rank: " << decoder.rank() << std::endl;
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
