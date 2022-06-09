// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <cmath>
#include <ctime>
#include <vector>

#include <benchmark/benchmark.h>

#include <fifi/field/prime2325_binary_search.hpp>
#include <fifi/field/prime2325_bitmap.hpp>

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar

static void prime2325_bitmap(benchmark::State& state)
{
    uint32_t length = state.range(0);
    state.counters["length"] = length;

    std::vector<uint8_t> block(length * sizeof(uint32_t));
    std::generate(block.begin(), block.end(), rand);

    fifi::field::prime2325_bitmap bitmap_search(length);

    for (auto _ : state)
    {
        volatile uint32_t prefix =
            bitmap_search.find_prefix(block.data(), length);
        (void)prefix;
    }
    state.SetBytesProcessed(block.size() * state.iterations());
}

BENCHMARK(prime2325_bitmap)
    ->DenseRange(1000, 1750000, 500000)
    ->Unit(benchmark::kMicrosecond);

/// This test shows how you can create configurations
/// for the benchmarks. This could be different string
/// lengths or similar

void prime2325_binary_search(benchmark::State& state)
{
    uint32_t length = state.range(0);
    uint32_t k = state.range(1);

    state.counters["length"] = length;
    state.counters["k"] = k;

    std::vector<uint8_t> block(length * sizeof(uint32_t));
    std::generate(block.begin(), block.end(), rand);

    fifi::field::prime2325_binary_search search(length, k);

    for (auto _ : state)
    {
        volatile uint32_t prefix = search.find_prefix(block.data(), length);
        (void)prefix;
    }
    state.SetBytesProcessed(block.size() * state.iterations());
}

static void CustomArguments(benchmark::internal::Benchmark* b)
{
    for (uint32_t i = 1000; i < 1750000; i += 500000)
    {
        for (uint32_t j = 1; j < 5; j++)
        {
            b->Args({i, j});
        }
    }
}

BENCHMARK(prime2325_binary_search)
    ->Apply(CustomArguments)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
