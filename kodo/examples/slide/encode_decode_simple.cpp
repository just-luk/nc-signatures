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
#include <iostream>
#include <vector>

// The first thing we need to do is to include the
// relevant header files.

// Encoder and decoder
#include <kodo/slide/decoder.hpp>
#include <kodo/slide/encoder.hpp>

// Coefficient generator
#include <kodo/slide/generator/random_uniform.hpp>

// Rate controller
#include <kodo/slide/rate_controller.hpp>

// A tiny helper struct for containing the state.
struct state
{
    uint32_t generated;
    uint32_t decoded;
    kodo::slide::decoder& decoder;
    kodo::slide::encoder& encoder;
    bool decoding_failed = false;
};

int main()
{
    // Set the window size (this is the maximum number of symbols included
    // in each encoded symbol).
    std::size_t window_symbols = 20U;

    // Set the capacity of the decoder (this is the number of
    // encoded symbols that are used in the decoding process).
    std::size_t decoder_capacity = 40U;

    // The finite field
    kodo::finite_field field = kodo::finite_field::binary8;

    // The maximum size of a symbol in bytes
    std::size_t max_symbol_bytes = 160U;

    // Create the encoder and decoder
    kodo::slide::encoder encoder(field);
    kodo::slide::decoder decoder(field);

    encoder.configure(max_symbol_bytes);
    decoder.configure(max_symbol_bytes);

    // Create generator
    kodo::slide::generator::random_uniform generator(field);

    // Buffer needed to store coding coefficients.
    std::vector<uint8_t> coefficients;

    // Use the rate controller to control the amount of repair/redundancy
    // generated:
    // n: Total number of symbols
    // k: Number of source symbols out of the total
    //
    // To get a good decoding performance the coding rate needs to be lower than
    // or equal to the packet loss probability.
    //
    // In this case our rate is k/n = 4/10 = 40% and our packet loss probability
    // is 50% so this should be sufficient for a good decoding performance.
    //
    // The above serves as a rule of thumb - more rigorous mathematics can be
    // used to derive tighter bounds given a certain decoding probability.
    uint32_t n = 10;
    uint32_t k = 4;
    kodo::slide::rate_controller rate{n, k};

    // Number of interations
    std::size_t iterations = 2000U;

    // State object for keeping track of the number of source and coded symbols
    state s{0, 0, decoder, encoder};

    // Set a callback for when a symbol is decoded
    decoder.on_symbol_decoded(
        [](uint64_t index, void* user_data)
        {
            std::cout << "Decoded index = " << index;
            state* s = static_cast<state*>(user_data);
            s->decoded++;

            if (s->encoder.in_stream(index))
            {
                if (std::memcmp(s->decoder.symbol_data(index),
                                s->encoder.symbol_data(index),
                                s->encoder.symbol_bytes(index)) == 0)
                {
                    // Decoding succeeded!
                    std::cout << " ✓\n";
                }
                else
                {
                    // Decoding failed! This should never happen.
                    std::cout << " ✕\n";
                    s->decoding_failed = true;
                }
            }
            else
            {
                // Unable to check if the decoding succeeded as the symbol is no
                // longer in the available in the input stream.
                std::cout << " ?\n";
            }
        },
        &s);

    while (iterations != 0)
    {
        --iterations;

        // Check if we should generate a repair symbol
        if (!rate.send_repair())
        {
            // Manage the encoder's window
            if (encoder.stream_symbols() == window_symbols)
            {
                // If window is full - pop a symbol before pushing a new one
                const uint8_t* symbol = encoder.pop_symbol();
                // Make sure to free the symbol
                delete[] symbol;
            }

            // Create a new source symbol
            uint8_t* symbol = new uint8_t[max_symbol_bytes];

            // In a real world application we would be encoding and decoding
            // symbols from some source, such as a video feed or network card.
            // In this example we will just imagine having such a source and
            // produce some random data.
            std::generate(symbol, symbol + max_symbol_bytes, rand);

            // Add the symbol's memory to the encoder
            encoder.push_symbol(symbol, max_symbol_bytes);
            s.generated++;
        }

        // Get encoding window
        kodo::slide::range window = encoder.stream_range();

        // Resize coefficients buffer
        coefficients.resize(generator.coefficients_bytes(window));

        // Choose a random seed for the encoding
        uint64_t seed = rand();

        // Set the seed used to generate the coding coefficients
        generator.set_seed(seed);

        // Generate the coding coefficients
        generator.generate(coefficients.data(), window);

        // Encode the symbol
        uint8_t* coded_symbol = new uint8_t[encoder.max_symbol_bytes()];
        auto size =
            encoder.encode_symbol(coded_symbol, window, coefficients.data());

        // Update loop state
        rate.advance();

        if (rand() % 2)
        {
            // Simulate 50% packet loss
            continue;
        }

        // Move the decoders's stream if needed
        //
        // If the encoded symbol includes symbols that the decoder does not
        // have. We need to update the state of the decoder.
        // In the following we will go through two cases:
        while (decoder.stream_upper_bound() < encoder.stream_upper_bound())
        {
            // If the decoder's capacity is full a symbol needs to be dropped
            // before a new one can be added.
            if (decoder.stream_symbols() == decoder_capacity)
            {
                // Remove the "oldest" symbol
                uint8_t* symbol = decoder.pop_symbol();

                // Free the symbol if it was allocated
                if (symbol != nullptr)
                {
                    delete[] symbol;
                }
            }
            assert(decoder.stream_symbols() < decoder_capacity);

            // Prepare decoder for a new symbol
            decoder.push_symbol();
        }

        // Set the seed and window on the generator
        generator.set_seed(seed);

        // Resize coefficients buffer
        coefficients.resize(generator.coefficients_bytes(window));

        // Generate the coding coefficients
        generator.generate(coefficients.data(), window);

        // Decode the symbol
        coded_symbol = decoder.decode_symbol(coded_symbol, size, window,
                                             coefficients.data());
        // Check if the symbol was claimed by the decoder
        if (coded_symbol != nullptr)
        {
            // Free the coded symbol if it's no longer needed by the decoder.
            delete[] coded_symbol;
        }
    }

    if (s.decoding_failed)
    {

        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
        return 1;
    }

    std::cout << "Decoding successful!\n";
    std::cout << "Generated symbols = " << s.generated << "\n";
    std::cout << "Decoded symbols = " << s.decoded << "\n";
    return 0;
}
