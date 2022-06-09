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

#include <cstdint>
#include <vector>

#include <benchmark/benchmark.h>

#include <kodo/detail/perpetual/inner_generator.hpp>
#include <kodo/detail/perpetual/stack_decoder.hpp>
#include <kodo/detail/perpetual/stack_encoder.hpp>
#include <kodo/perpetual/decoder.hpp>
#include <kodo/perpetual/encoder.hpp>
#include <kodo/perpetual/generator/random_uniform.hpp>
#include <kodo/perpetual/offset/random_uniform.hpp>
#include <kodo/to_string.hpp>

using encoder_type = kodo::perpetual::encoder;
using decoder_type = kodo::perpetual::decoder;
using generator_type = kodo::perpetual::generator::random_uniform;
using offset_generator_type = kodo::perpetual::offset::random_uniform;

/// Small struct representing a symbol sent over the wire in the perpetual
/// code.
struct coded_symbol
{
    coded_symbol(std::size_t symbol_bytes) : payload(symbol_bytes, 0)
    {
        assert(symbol_bytes > 0);
    }

    /// The offset
    std::size_t offset;

    /// The coefficients
    uint64_t coefficients;

    /// The encoded symbol payload
    std::vector<uint8_t> payload;
};

void generate_coded_symbols(encoder_type& encoder, generator_type& generator,
                            offset_generator_type& offset_generator,
                            std::vector<coded_symbol>& symbols_out)
{
    std::size_t count = 0;
    for (auto& symbol : symbols_out)
    {
        symbol.offset = offset_generator.offset();
        symbol.coefficients = generator.generate(count++);
        encoder.encode_symbol(symbol.payload.data(), symbol.coefficients,
                              symbol.offset);
    }
}

static void perpetual_encode(benchmark::State& state,
                             kodo::perpetual::width width,
                             std::size_t block_bytes, std::size_t symbol_bytes,
                             std::size_t outer_interval,
                             std::size_t outer_segments)
{
    state.counters["block_bytes"] = block_bytes;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["outer_interval"] = outer_interval;
    state.counters["outer_segments"] = outer_segments;

    encoder_type encoder(width);
    generator_type generator(width);
    offset_generator_type offset_generator;
    encoder.configure(block_bytes, symbol_bytes, outer_interval,
                      outer_segments);
    offset_generator.configure(encoder.symbols());

    state.counters["width"] = encoder.width();

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());

    std::vector<coded_symbol> coded_symbols(
        encoder.symbols(), coded_symbol{encoder.symbol_bytes()});

    for (auto _ : state)
    {
        generate_coded_symbols(encoder, generator, offset_generator,
                               coded_symbols);
    }

    state.SetBytesProcessed(encoder.block_bytes() * state.iterations());
}

static void perpetual_decode(benchmark::State& state,
                             kodo::perpetual::width width,
                             std::size_t block_bytes, std::size_t symbol_bytes,
                             std::size_t outer_interval,
                             std::size_t outer_segments,
                             double mapping_threshold)
{
    state.counters["block_bytes"] = block_bytes;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["outer_interval"] = outer_interval;
    state.counters["outer_segments"] = outer_segments;

    encoder_type encoder(width);
    decoder_type decoder(width);
    generator_type generator(width);
    offset_generator_type offset_generator;

    encoder.configure(block_bytes, symbol_bytes, outer_interval,
                      outer_segments);
    decoder.configure(block_bytes, symbol_bytes, outer_interval, outer_segments,
                      mapping_threshold);

    offset_generator.configure(encoder.symbols());
    state.counters["width"] = encoder.width();
    state.counters["symbols"] = encoder.symbols();
    state.counters["data_symbols"] = encoder.data_symbols();

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());

    std::vector<coded_symbol> coded_symbols(
        encoder.symbols() * 1.5, coded_symbol{encoder.symbol_bytes()});

    for (auto _ : state)
    {
        state.PauseTiming();

        decoder.reset();
        std::vector<uint8_t> data_out(decoder.block_bytes());
        decoder.set_symbols_storage(data_out.data());
        generate_coded_symbols(encoder, generator, offset_generator,
                               coded_symbols);

        state.ResumeTiming();
        for (auto& symbol : coded_symbols)
        {
            decoder.decode_symbol(symbol.payload.data(), symbol.coefficients,
                                  symbol.offset);

            state.counters["symbols_processed"] += 1;

            if (decoder.is_complete())
            {
                break;
            }
        }
        state.counters["complete"] = decoder.is_complete();
        state.counters["overhead"] = (state.counters["symbols_processed"] /
                                      state.counters["data_symbols"]) -
                                     1;

        if (!decoder.is_complete())
        {
            state.SkipWithError("Failed to decode data!");
        }
    }

    state.SetBytesProcessed(decoder.block_bytes() * state.iterations());
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kMillisecond);
    b->Repetitions(5);
}

BENCHMARK_CAPTURE(perpetual_encode, size_256_mb, kodo::perpetual::width::_32,
                  256000000, 1400, 8, 8)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(perpetual_encode, size_1_gb, kodo::perpetual::width::_32,
                  1000000000, 1400, 8, 8)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(perpetual_decode, size_256_mb, kodo::perpetual::width::_32,
                  256000000, 1400, 8, 8, 0.98)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(perpetual_decode, size_1_gb, kodo::perpetual::width::_32,
                  1000000000, 1400, 8, 8, 0.98)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
