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

#include <cstdint>
#include <numeric>

#include <benchmark/benchmark.h>

#include <fifi/math.hpp>

static void BM_binary(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Create a new instance of the field implementation
        volatile fifi::math math{fifi::finite_field::binary};
        (void)math;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_binary)->Unit(benchmark::kMicrosecond);

static void BM_binary4(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Create a new instance of the field implementation
        volatile fifi::math math{fifi::finite_field::binary4};
        (void)math;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_binary4)->Unit(benchmark::kMicrosecond);

static void BM_binary8(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Create a new instance of the field implementation
        volatile fifi::math math{fifi::finite_field::binary8};
        (void)math;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_binary8)->Unit(benchmark::kMicrosecond);

static void BM_binary16(benchmark::State& state)
{
    for (auto _ : state)
    {
        // Create a new instance of the field implementation
        volatile fifi::math math{fifi::finite_field::binary16};
        (void)math;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_binary16)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
