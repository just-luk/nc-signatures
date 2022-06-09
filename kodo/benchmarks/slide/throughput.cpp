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

#include <kodo/slide/decoder.hpp>
#include <kodo/slide/encoder.hpp>
#include <kodo/slide/generator/random_uniform.hpp>
#include <kodo/to_string.hpp>

struct coded_symbol
{
    // Window
    kodo::slide::range window;

    /// The seed for the random generator
    uint64_t seed = 0;

    /// The symbol payload
    std::vector<uint8_t> payload;
};

struct systematic_symbol
{
    // index
    uint64_t index;

    /// The symbol payload
    std::vector<uint8_t> payload;
};

void generate_coded_symbols(kodo::slide::encoder& encoder,
                            kodo::slide::generator::random_uniform& generator,
                            std::vector<uint8_t>& coefficients,
                            std::vector<coded_symbol>& symbols_out)
{
    for (auto& symbol : symbols_out)
    {
        generator.set_seed(symbol.seed);
        generator.generate(coefficients.data(), symbol.window);
        encoder.encode_symbol(symbol.payload.data(), symbol.window,
                              coefficients.data());
    }
}

void generate_systematic_symbols(kodo::slide::encoder& encoder,
                                 std::vector<systematic_symbol>& symbols_out)
{
    for (auto& symbol : symbols_out)
    {
        encoder.encode_systematic_symbol(symbol.payload.data(), symbol.index);
    }
}

static void slide_encode_coded(benchmark::State& state,
                               kodo::finite_field field, std::size_t symbols,
                               std::size_t symbol_bytes)
{
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = false;

    kodo::slide::encoder encoder{field};
    kodo::slide::generator::random_uniform generator{field};

    encoder.configure(symbol_bytes);

    std::vector<uint8_t> data_in(symbols * symbol_bytes);
    std::generate(data_in.begin(), data_in.end(), rand);

    std::vector<coded_symbol> coded_symbols;
    for (std::size_t i = 0; i < symbols; ++i)
    {
        encoder.push_symbol(data_in.data() + (i * symbol_bytes), symbol_bytes);

        coded_symbols.emplace_back(
            coded_symbol{kodo::slide::range{0, symbols}, i,
                         std::vector<uint8_t>(symbol_bytes)});
    }

    std::vector<uint8_t> coefficients(
        generator.coefficients_bytes(kodo::slide::range{0, symbols}));
    for (auto _ : state)
    {
        generate_coded_symbols(encoder, generator, coefficients, coded_symbols);
    }

    state.SetBytesProcessed(symbols * symbol_bytes * state.iterations());
}

static void slide_encode_systematic(benchmark::State& state,
                                    std::size_t symbols,
                                    std::size_t symbol_bytes)
{
    kodo::finite_field field = kodo::finite_field::binary;
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = true;

    kodo::slide::encoder encoder{field};

    encoder.configure(symbol_bytes);

    std::vector<uint8_t> data_in(symbols * symbol_bytes);
    std::generate(data_in.begin(), data_in.end(), rand);

    std::vector<systematic_symbol> systematic_symbols;
    for (std::size_t i = 0; i < symbols; ++i)
    {
        encoder.push_symbol(data_in.data() + (i * symbol_bytes), symbol_bytes);

        systematic_symbols.emplace_back(
            systematic_symbol{i, std::vector<uint8_t>(symbol_bytes)});
    }

    for (auto _ : state)
    {
        for (auto& symbol : systematic_symbols)
        {
            benchmark::DoNotOptimize(symbol.payload.data());
            encoder.encode_systematic_symbol(symbol.payload.data(),
                                             symbol.index);
            benchmark::ClobberMemory();
        }
    }

    state.SetBytesProcessed(symbols * symbol_bytes * state.iterations());
}

static void slide_decode_coded(benchmark::State& state,
                               kodo::finite_field field, std::size_t symbols,
                               std::size_t symbol_bytes)
{
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = false;

    kodo::slide::encoder encoder{field};
    kodo::slide::decoder decoder{field};
    kodo::slide::generator::random_uniform generator{field};

    encoder.configure(symbol_bytes);
    decoder.configure(symbol_bytes);

    std::vector<uint8_t> coefficients(
        generator.coefficients_bytes(kodo::slide::range{0, symbols}));

    std::vector<uint8_t> data_in(symbols * symbol_bytes);
    std::generate(data_in.begin(), data_in.end(), rand);

    std::vector<coded_symbol> coded_symbols;
    for (std::size_t i = 0; i < symbols; ++i)
    {
        encoder.push_symbol(data_in.data() + (i * symbol_bytes), symbol_bytes);
    }

    for (std::size_t i = 0; i < symbols * 2; ++i)
    {
        coded_symbols.emplace_back(
            coded_symbol{kodo::slide::range{0, symbols}, i,
                         std::vector<uint8_t>(symbol_bytes)});
    }

    for (auto _ : state)
    {
        state.PauseTiming();

        decoder.reset();
        for (std::size_t i = 0; i < symbols; ++i)
            decoder.push_symbol();

        generate_coded_symbols(encoder, generator, coefficients, coded_symbols);

        state.ResumeTiming();
        for (auto& symbol : coded_symbols)
        {
            generator.set_seed(symbol.seed);
            generator.generate(coefficients.data(), symbol.window);
            decoder.decode_symbol(symbol.payload.data(), symbol.payload.size(),
                                  symbol.window, coefficients.data());

            state.counters["symbols_processed"] += 1;

            if (decoder.symbols_decoded() == symbols)
            {
                break;
            }
        }
        if (decoder.symbols_decoded() != symbols)
        {
            state.SkipWithError("Failed to decode data!");
        }
    }

    state.SetBytesProcessed(symbols * symbol_bytes * state.iterations());
}

static void slide_decode_systematic(benchmark::State& state,
                                    std::size_t symbols,
                                    std::size_t symbol_bytes)
{
    kodo::finite_field field = kodo::finite_field::binary;
    state.SetLabel(kodo::to_string(field));
    state.counters["symbols"] = symbols;
    state.counters["symbol_bytes"] = symbol_bytes;
    state.counters["systematic"] = true;

    kodo::slide::decoder decoder{field};
    kodo::slide::encoder encoder{field};

    encoder.configure(symbol_bytes);
    decoder.configure(symbol_bytes);

    std::vector<uint8_t> data_in(symbols * symbol_bytes);
    std::generate(data_in.begin(), data_in.end(), rand);

    std::vector<systematic_symbol> systematic_symbols;
    for (std::size_t i = 0; i < symbols; ++i)
    {
        encoder.push_symbol(data_in.data() + (i * symbol_bytes), symbol_bytes);

        systematic_symbols.emplace_back(
            systematic_symbol{i, std::vector<uint8_t>(symbol_bytes)});
    }

    for (auto _ : state)
    {
        state.PauseTiming();

        decoder.reset();
        for (std::size_t i = 0; i < symbols; ++i)
            decoder.push_symbol();

        state.ResumeTiming();
        for (auto& symbol : systematic_symbols)
        {
            decoder.decode_systematic_symbol(
                symbol.payload.data(), symbol.payload.size(), symbol.index);
        }
        if (decoder.symbols_decoded() != symbols)
        {
            state.SkipWithError("Failed to decode data!");
        }
    }

    state.SetBytesProcessed(symbols * symbol_bytes * state.iterations());
}

static void BenchmarkArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kMillisecond);
    b->Repetitions(5);
}

BENCHMARK_CAPTURE(slide_encode_systematic, systematic, 200, 1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(slide_decode_systematic, systematic, 200, 1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(slide_encode_coded, binary, kodo::finite_field::binary, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(slide_encode_coded, binary4, kodo::finite_field::binary4, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(slide_encode_coded, binary8, kodo::finite_field::binary8, 200,
                  1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_CAPTURE(slide_decode_coded, binary, kodo::finite_field::binary, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(slide_decode_coded, binary4, kodo::finite_field::binary4, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(slide_decode_coded, binary8, kodo::finite_field::binary8, 200,
                  1400)
    ->Apply(BenchmarkArguments);
BENCHMARK_CAPTURE(slide_decode_coded, binary8, kodo::finite_field::binary8, 20,
                  1400)
    ->Apply(BenchmarkArguments);

BENCHMARK_MAIN();
