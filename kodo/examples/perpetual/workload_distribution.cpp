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
#include <iomanip>
#include <iostream>
#include <vector>

#include <kodo/perpetual/decoder.hpp>
#include <kodo/perpetual/encoder.hpp>
#include <kodo/perpetual/generator/random_uniform.hpp>
#include <kodo/perpetual/offset/random_uniform.hpp>

/// @example perpetual/work_load_distribution.cpp
///
/// Simple example which prints out the time used in various intervals of the
/// encode/decode process
int main()
{
    // Seed the random number generator to get different random data
    srand(static_cast<uint32_t>(time(0)));

    kodo::perpetual::encoder encoder(kodo::perpetual::width::_32);
    kodo::perpetual::decoder decoder(kodo::perpetual::width::_32);

    auto block_bytes = 1000000000;
    auto symbol_bytes = 1400;

    // bucket interval
    std::size_t interval = 10000;

    // Configure the coders.
    encoder.configure(block_bytes, symbol_bytes);
    decoder.configure(block_bytes, symbol_bytes);

    // Create generator and offset generator.
    kodo::perpetual::generator::random_uniform generator(
        kodo::perpetual::width::_32);
    kodo::perpetual::offset::random_uniform offset_generator;

    // Configure offset generator.
    offset_generator.configure(encoder.symbols());

    // Allocate some storage for a symbol.
    std::vector<uint8_t> symbol(encoder.symbol_bytes());

    // Allocate some data to encode.
    std::vector<uint8_t> data_in(encoder.block_bytes());

    // Just for fun - fill data_in with random data
    std::generate(data_in.begin(), data_in.end(), rand);

    // Assign the data buffer to the encoder so that we may start
    // to produce encoded symbols from it
    encoder.set_symbols_storage(data_in.data());

    // Define a data buffer where the symbols should be decoded
    std::vector<uint8_t> data_out(decoder.block_bytes());
    decoder.set_symbols_storage(data_out.data());

    std::size_t last_rank = 0;
    std::size_t symbols_generated_in_interval = 0U;
    std::size_t total_overhead = 0;
    double total_time_ms = 0.0;

    /// We define work as the difference between the offset of a symbol
    /// and the pivot position it takes
    std::size_t work = 0;
    std::size_t total_work = 0;

    // The number of symbols that get directly inserted
    std::size_t inserted_symbols = 0;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    auto start_time = std::chrono::steady_clock::now();

    std::size_t next_interval = interval;
    std::size_t seed = rand();

    while (!decoder.is_complete())
    {
        // Generate the offset
        auto offset = offset_generator.offset();
        auto coefficients = generator.generate(seed++);

        encoder.encode_symbol(symbol.data(), coefficients, offset);
        decoder.decode_symbol(symbol.data(), coefficients, offset);

        if (decoder.pivot_found())
        {
            work += decoder.pivot() > offset ? decoder.pivot() - offset
                                             : offset - decoder.pivot();
            inserted_symbols += (offset == decoder.pivot());
        }

        symbols_generated_in_interval += 1U;

        if ((decoder.rank() == next_interval))
        {
            double time_ms = std::chrono::duration<double, std::milli>(
                                 std::chrono::steady_clock::now() - start_time)
                                 .count();

            std::size_t overhead =
                symbols_generated_in_interval - (decoder.rank() - last_rank);

            total_overhead += overhead;
            total_time_ms += time_ms;
            total_work += work;

            std::cout << std::setw(6) << last_rank << " -> ";
            std::cout << std::setw(6) << decoder.rank() << " : ";
            std::cout << std::setw(10) << time_ms << "ms";
            std::cout << std::setw(6) << overhead << " overhead";
            std::cout << std::setw(8) << work << " work\n";

            start_time = std::chrono::steady_clock::now();
            last_rank = decoder.rank();
            symbols_generated_in_interval = 0U;
            next_interval += interval;
            work = 0U;
        }
    }

    std::cout << "Total overhead " << total_overhead << " symbols ("
              << (total_overhead / (double)encoder.data_symbols()) * 100 << "%)"
              << std::endl;
    std::cout << "Total time " << total_time_ms << " ms" << std::endl;
    std::cout << "Total work " << total_work << std::endl;
    std::cout << "Total inserted " << inserted_symbols << " out of "
              << decoder.symbols() << std::endl;

    // Check if we properly decoded the data
    if (data_out != data_in)
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
        return 1;
    }

    return 0;
}