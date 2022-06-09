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

#include <kodo/block/decoder.hpp>
#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>
#include <kodo/to_string.hpp>

/// Small struct representing a symbol sent over the wire in the perpetual
/// code.
struct coded_symbol
{
    coded_symbol(std::size_t symbol_bytes) : payload(symbol_bytes, 0)
    {
        assert(symbol_bytes > 0);
    }

    /// The seed for the random generator
    uint64_t seed = 0;

    /// The encoded symbol payload
    std::vector<uint8_t> payload;
};

void generate_coded_symbols(kodo::block::encoder& encoder,
                            kodo::block::generator::random_uniform& generator,
                            std::vector<uint8_t>& coefficients,
                            std::vector<coded_symbol>& symbols_out)
{
    for (auto& symbol : symbols_out)
    {
        symbol.seed = rand();
        generator.set_seed(symbol.seed);
        generator.generate(coefficients.data());
        encoder.encode_symbol(symbol.payload.data(), coefficients.data());
    }
}

void generate_systematic_symbols(kodo::block::encoder& encoder,
                                 std::vector<std::vector<uint8_t>>& symbols_out)
{
    for (size_t i = 0; i < symbols_out.size(); i++)
    {
        encoder.encode_systematic_symbol(symbols_out.at(i).data(), i);
    }
}

static void block_encode_coded(benchmark::State& state,
                               kodo::finite_field field, std::size_t symbols,
                               std::size_t symbol_bytes)
{
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = false;

    kodo::block::encoder encoder{field};
    kodo::block::generator::random_uniform generator{field};

    encoder.configure(symbols, symbol_bytes);
    generator.configure(encoder.symbols());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());

    std::vector<coded_symbol> coded_symbols(
        encoder.symbols(), coded_symbol{encoder.symbol_bytes()});

    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
    for (auto _ : state)
    {
        generate_coded_symbols(encoder, generator, coefficients, coded_symbols);
    }

    state.SetBytesProcessed(encoder.block_bytes() * state.iterations());
}

static void block_encode_systematic(benchmark::State& state,
                                    std::size_t symbols,
                                    std::size_t symbol_bytes)
{
    kodo::finite_field field = kodo::finite_field::binary;
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = true;

    kodo::block::encoder encoder{field};

    encoder.configure(symbols, symbol_bytes);

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());

    std::vector<std::vector<uint8_t>> systematic_symbols(
        encoder.symbols(), std::vector<uint8_t>(encoder.symbol_bytes(), 0));

    for (auto _ : state)
    {
        std::size_t index = 0;
        for (auto& symbol : systematic_symbols)
        {
            benchmark::DoNotOptimize(symbol.data());
            encoder.encode_systematic_symbol(symbol.data(), index);
            benchmark::ClobberMemory();
            index += 1;
        }
    }

    state.SetBytesProcessed(encoder.block_bytes() * state.iterations());
}

static void block_decode_coded(benchmark::State& state,
                               kodo::finite_field field, std::size_t symbols,
                               std::size_t symbol_bytes)
{
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = false;

    kodo::block::encoder encoder{field};
    kodo::block::decoder decoder{field};
    kodo::block::generator::random_uniform generator{field};

    encoder.configure(symbols, symbol_bytes);
    decoder.configure(symbols, symbol_bytes);
    generator.configure(encoder.symbols());

    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    encoder.set_symbols_storage(data_in.data());

    std::vector<coded_symbol> coded_symbols(
        encoder.symbols() * 2, coded_symbol{encoder.symbol_bytes()});

    for (auto _ : state)
    {
        state.PauseTiming();

        decoder.reset();
        std::vector<uint8_t> data_out(decoder.block_bytes());
        decoder.set_symbols_storage(data_out.data());
        std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());
        generate_coded_symbols(encoder, generator, coefficients, coded_symbols);

        state.ResumeTiming();
        for (auto& symbol : coded_symbols)
        {
            generator.set_seed(symbol.seed);
            generator.generate(coefficients.data());
            decoder.decode_symbol(symbol.payload.data(), coefficients.data());

            state.counters["symbols_processed"] += 1;

            if (decoder.is_complete())
            {
                break;
            }
        }
        if (!decoder.is_complete())
        {
            state.SkipWithError("Failed to decode data!");
        }
    }

    state.SetBytesProcessed(decoder.block_bytes() * state.iterations());
}

static void block_decode_systematic(benchmark::State& state,
                                    std::size_t symbols,
                                    std::size_t symbol_bytes)
{
    kodo::finite_field field = kodo::finite_field::binary;
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = true;

    kodo::block::decoder decoder{field};
    kodo::block::encoder encoder{field};

    encoder.configure(symbols, symbol_bytes);
    decoder.configure(symbols, symbol_bytes);

    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);
    encoder.set_symbols_storage(data_in.data());

    std::vector<std::vector<uint8_t>> systematic_symbols(
        encoder.symbols(), std::vector<uint8_t>(encoder.symbol_bytes(), 0));
    generate_systematic_symbols(encoder, systematic_symbols);

    for (auto _ : state)
    {
        state.PauseTiming();

        decoder.reset();
        std::vector<uint8_t> data_out(decoder.block_bytes());
        decoder.set_symbols_storage(data_out.data());

        state.ResumeTiming();
        std::size_t index = 0;
        for (const auto& symbol : systematic_symbols)
        {
            decoder.decode_systematic_symbol(symbol.data(), index);
            index += 1;
        }
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

BENCHMARK_CAPTURE(block_encode_systematic, systematic, 200, 1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(block_decode_systematic, systematic, 200, 1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(block_encode_coded, binary, kodo::finite_field::binary, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(block_encode_coded, binary4, kodo::finite_field::binary4, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(block_encode_coded, binary8, kodo::finite_field::binary8, 200,
                  1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(block_decode_coded, binary, kodo::finite_field::binary, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(block_decode_coded, binary4, kodo::finite_field::binary4, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(block_decode_coded, binary8, kodo::finite_field::binary8, 200,
                  1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
