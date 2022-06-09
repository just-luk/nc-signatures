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

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <limits>
#include <type_traits>
#include <vector>

#include <cpuid/cpuinfo.hpp>

#include <benchmark/benchmark.h>

#include <fifi/field/binary4_avx2.hpp>
#include <fifi/field/binary4_basic.hpp>
#include <fifi/field/binary4_math.hpp>
#include <fifi/field/binary4_ssse3.hpp>
#include <fifi/field/binary8_avx2.hpp>
#include <fifi/field/binary8_basic.hpp>
#include <fifi/field/binary8_ssse3.hpp>
#include <fifi/field/binary_avx2.hpp>
#include <fifi/field/binary_basic.hpp>
#include <fifi/field/binary_math.hpp>
#include <fifi/field/binary_neon.hpp>
#include <fifi/field/binary_sse2.hpp>
#include <fifi/finite_field.hpp>
#include <fifi/math.hpp>
#include <fifi/to_string.hpp>
#include <fifi/utils.hpp>

namespace
{

int _binary8_prime = 0x11D;

template <class Math>
static auto check_cpu(const Math&, benchmark::State& state) -> bool
{
    bool result = false;
    cpuid::cpuinfo cpuid;
    switch (Math::simd_type)
    {
    case fifi::simd::no_simd:
        return true;
    case fifi::simd::sse2:
        result = cpuid.has_sse2();
        break;
    case fifi::simd::ssse3:
        result = cpuid.has_sse3();
        break;
    case fifi::simd::sse42:
        result = cpuid.has_sse4_2();
        break;
    case fifi::simd::avx2:
        result = cpuid.has_avx2();
        break;
    case fifi::simd::neon:
        result = cpuid.has_neon();
        break;
    }
    if (!result)
        state.SkipWithError("Unsupported CPU instruction");
    return result;
}

static auto check_cpu(const fifi::math&, benchmark::State&) -> bool
{
    return true;
}

static auto random_nonzero_constant(fifi::finite_field field) -> uint32_t
{
    return 1 + (std::rand() % (fifi::max_value(field) - 1));
}

static void CustomArguments(benchmark::internal::Benchmark* b)
{
    b->Args({1600});
    b->Unit(benchmark::kMicrosecond);
}

static void VectorDotArguments(benchmark::internal::Benchmark* b)
{
    b->Args({64, 1600});
    b->Unit(benchmark::kMicrosecond);
}

/// The vector dot product function is basically a bulk version of
/// vector_multiply_add_into.
/// This dummy struct overwrites fifi::maths vector_dot_product with and
/// implementation using vector_multiply_add_into.
/// The comparision of this vs fifi::maths vector_dot_product shows when the
/// usage of vector_dot_product outperformance multiple calls to
/// vector_multiply_add_into.
struct vector_dot_product_reference_math : fifi::math
{
    vector_dot_product_reference_math(fifi::finite_field field) :
        fifi::math(field)
    {
    }

    void vector_dot_product(uint8_t** x, const uint8_t** y,
                            const uint8_t** constants, std::size_t size,
                            std::size_t x_vectors, std::size_t y_vectors) const
    {
        for (std::size_t i = 0; i < x_vectors; ++i)
        {
            std::fill_n(x[i], size, 0);

            for (std::size_t j = 0; j < y_vectors; ++j)
            {
                uint32_t constant = fifi::math::get_value(constants[i], j);
                fifi::math::vector_multiply_add_into(x[i], y[j], constant,
                                                     size);
            }
        }
    }
};

static auto check_cpu(const vector_dot_product_reference_math&,
                      benchmark::State&) -> bool
{
    return true;
}

template <class Math>
static void BM_binary_vector_add_into(benchmark::State& state, const Math& math)
{
    if (!check_cpu(math, state))
        return;

    auto size = state.range(0);
    state.counters["size"] = size;
    state.SetLabel("binary");

    std::vector<uint8_t> x(size);
    std::vector<uint8_t> y(size);
    std::generate(x.begin(), x.end(), rand);
    std::generate(y.begin(), y.end(), rand);

    for (auto _ : state)
    {
        math.vector_add_into(x.data(), y.data(), size);
    }

    state.SetBytesProcessed(size * state.iterations());
}

template <class Math>
static void BM_vector_multiply_add_into(benchmark::State& state,
                                        const Math& math)
{
    if (!check_cpu(math, state))
        return;

    auto size = state.range(0);
    state.counters["size"] = size;

    auto field = math.field();
    state.SetLabel(fifi::to_string(field));

    std::vector<uint8_t> x(size);
    std::vector<uint8_t> y(size);
    std::generate(x.begin(), x.end(), rand);
    std::generate(y.begin(), y.end(), rand);
    uint32_t constant = random_nonzero_constant(field);

    for (auto _ : state)
    {
        math.vector_multiply_add_into(x.data(), y.data(), constant, size);
    }

    state.SetBytesProcessed(size * state.iterations());
}

template <class Math>
static void BM_vector_dot_product(benchmark::State& state, const Math& math)
{
    if (!check_cpu(math, state))
        return;

    auto field = math.field();

    std::size_t vectors = state.range(0);
    std::size_t size = state.range(1);
    state.counters["size"] = size;
    state.counters["vectors"] = vectors;
    state.SetLabel(fifi::to_string(field));

    std::vector<uint8_t> xs(vectors * size);
    std::vector<uint8_t> ys(vectors * size);
    std::generate(xs.begin(), xs.end(), rand);
    std::generate(ys.begin(), ys.end(), rand);

    std::size_t constants_size =
        fifi::elements_to_bytes(field, vectors * vectors);
    std::vector<uint8_t> constants(constants_size);

    for (std::size_t i = 0; i < vectors * vectors; ++i)
    {
        fifi::set_value(field, constants.data(), i,
                        random_nonzero_constant(field));
    }

    std::vector<uint8_t*> xs_ptr(vectors);
    std::vector<const uint8_t*> ys_ptr(vectors);
    std::vector<const uint8_t*> constants_ptr(vectors);

    for (std::size_t i = 0; i < vectors; i++)
    {
        xs_ptr[i] = xs.data() + i * size;
        ys_ptr[i] = ys.data() + i * size;
        constants_ptr[i] =
            constants.data() + (i * fifi::elements_to_bytes(field, vectors));
    }

    for (auto _ : state)
    {
        math.vector_dot_product(xs_ptr.data(), ys_ptr.data(),
                                constants_ptr.data(), size, vectors, vectors);
    }

    state.SetBytesProcessed(size * vectors * vectors * state.iterations());
}
}

BENCHMARK_CAPTURE(BM_binary_vector_add_into, binary_basic,
                  fifi::field::binary_basic())
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(BM_binary_vector_add_into, binary_sse2,
                  fifi::field::binary_sse2())
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(BM_binary_vector_add_into, binary_avx2,
                  fifi::field::binary_avx2())
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(BM_binary_vector_add_into, binary_neon,
                  fifi::field::binary_neon())
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(BM_binary_vector_add_into, math_binary,
                  fifi::math(fifi::finite_field::binary))
    ->Apply(CustomArguments);

BENCHMARK_CAPTURE(
    BM_vector_multiply_add_into, binary8_basic,
    fifi::field::binary8_basic(fifi::field::binary8(_binary8_prime)))
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(
    BM_vector_multiply_add_into, binary8_ssse3,
    fifi::field::binary8_ssse3(fifi::field::binary8(_binary8_prime)))
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(
    BM_vector_multiply_add_into, binary8_neon,
    fifi::field::binary8_neon(fifi::field::binary8(_binary8_prime)))
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(
    BM_vector_multiply_add_into, binary8_avx2,
    fifi::field::binary8_avx2(fifi::field::binary8(_binary8_prime)))
    ->Apply(CustomArguments);
BENCHMARK_CAPTURE(BM_vector_multiply_add_into, math_binary8,
                  fifi::math(fifi::finite_field::binary8))
    ->Apply(CustomArguments);

BENCHMARK_CAPTURE(
    BM_vector_dot_product, binary8_basic,
    fifi::field::binary8_basic(fifi::field::binary8(_binary8_prime)))
    ->Apply(VectorDotArguments);
BENCHMARK_CAPTURE(
    BM_vector_dot_product, binary8_ssse3,
    fifi::field::binary8_ssse3(fifi::field::binary8(_binary8_prime)))
    ->Apply(VectorDotArguments);
BENCHMARK_CAPTURE(
    BM_vector_dot_product, binary8_neon,
    fifi::field::binary8_neon(fifi::field::binary8(_binary8_prime)))
    ->Apply(VectorDotArguments);
BENCHMARK_CAPTURE(
    BM_vector_dot_product, binary8_avx2,
    fifi::field::binary8_avx2(fifi::field::binary8(_binary8_prime)))
    ->Apply(VectorDotArguments);
BENCHMARK_CAPTURE(BM_vector_dot_product, math_binary8,
                  fifi::math(fifi::finite_field::binary8))
    ->Apply(VectorDotArguments);
BENCHMARK_CAPTURE(
    BM_vector_dot_product, math_binary8_reference,
    vector_dot_product_reference_math(fifi::finite_field::binary8))
    ->Apply(VectorDotArguments);

BENCHMARK_MAIN();
