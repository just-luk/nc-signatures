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

#include "binary4_math.hpp"
#include "bytes_to_elements.hpp"
#include "elements_to_bytes.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

#include "../detail/get_total_field_metrics.hpp"

#include <platform/config.hpp>

#include <vector>

#include <cpuid/cpuinfo.hpp>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

// Default backend implementation
static constexpr field::binary4_backend default_binary4{field::binary4()};

/// Initialize cpu support
static const detail::cpu_support cpu;

binary4_math::binary4_math() : binary4_math::binary4_math(&default_binary4)
{
}

binary4_math::binary4_math(const binary4_backend* backend) :
    binary4_math::binary4_math(backend, cpu)
{
}

binary4_math::binary4_math(const binary4_backend* backend,
                           const detail::cpu_support& cpu) :
    m_metrics(13, 32, "Field")
{
    set_backend(backend, cpu);
    m_field_metrics.m_no_simd = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_no_simd),
        "No SIMD");
    m_field_metrics.m_sse2 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_sse2), "SSE2");
    m_field_metrics.m_ssse3 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_ssse3), "SSSE3");
    m_field_metrics.m_sse42 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_sse42), "SSE4.2");
    m_field_metrics.m_avx2 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_avx2), "AVX2");
    m_field_metrics.m_neon = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_neon), "Neon");
    m_field_metrics.m_sse2_flag = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::has_sse2), "Has SSE2");
    m_field_metrics.m_ssse3_flag = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::has_ssse3), "Has SSSE3");
    m_field_metrics.m_sse42_flag = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::has_sse42), "Has SSE42");
    m_field_metrics.m_avx2_flag = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::has_avx2), "Has AVX2");
    m_field_metrics.m_neon_flag = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::has_neon), "Has Neon");

    m_field_metrics.m_sse2_flag += cpu.sse2;
    m_field_metrics.m_ssse3_flag += cpu.ssse3;
    m_field_metrics.m_sse42_flag += cpu.sse42;
    m_field_metrics.m_avx2_flag += cpu.avx2;
    m_field_metrics.m_neon_flag += cpu.neon;

    m_field_metrics.m_field_degree = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::field_degree), "Field Degree");

    m_field_metrics.m_field_polynomial = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::field_polynomial),
        "Field Polynomial");

    m_field_metrics.m_field_degree = m_backend->m_field.degree;
    m_field_metrics.m_field_polynomial = m_backend->m_field.prime();
}

void binary4_math::set_backend(const binary4_backend* backend)
{
    set_backend(backend, cpu);
}

void binary4_math::set_backend(const binary4_backend* backend,
                               const detail::cpu_support& cpu)
{
    assert(backend != nullptr);
    m_backend = backend;
    m_binary_avx2 = {&m_backend->m_binary_avx2, cpu.avx2};
    m_binary_sse2 = {&m_backend->m_binary_sse2, cpu.sse2};
    m_binary_neon = {&m_backend->m_binary_neon, cpu.neon};
    m_binary_basic = {&m_backend->m_binary_basic, true};
    m_binary4_basic = {&m_backend->m_binary4_basic, true};
    m_binary4_ssse3 = {&m_backend->m_binary4_ssse3, cpu.ssse3};
    m_binary4_avx2 = {&m_backend->m_binary4_avx2, cpu.avx2};
    m_binary4_neon = {&m_backend->m_binary4_neon, cpu.neon};

    auto check_prime = [](auto a, auto prime)
    { return !a.is_enabled() || a->field_info().prime() == prime; };

    // Check that all non-binary accelerations use the right prime
    assert(check_prime(m_binary4_basic, field_info().prime()));
    assert(check_prime(m_binary4_ssse3, field_info().prime()));
    assert(check_prime(m_binary4_avx2, field_info().prime()));
    assert(check_prime(m_binary4_neon, field_info().prime()));

    // Silence warnings if compiling without asserts
    (void)check_prime;
}

finite_field binary4_math::field() const
{
    return fifi::finite_field::binary4;
}

binary4 binary4_math::field_info() const
{
    assert(m_backend != nullptr);

    return m_backend->m_field;
}

uint32_t binary4_math::add(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary4_basic->add(a, b);
}

uint32_t binary4_math::subtract(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary4_basic->subtract(a, b);
}

uint32_t binary4_math::multiply(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary4_basic->multiply(a, b);
}

uint32_t binary4_math::divide(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary4_basic->divide(a, b);
}

uint32_t binary4_math::invert(uint32_t a) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary4_basic->invert(a);
}

void binary4_math::vector_add_into(uint8_t* x, const uint8_t* y,
                                   std::size_t size) const
{
    const auto vector_add_into = [&](auto& c, abacus::metric& metric)
    {
        if (c.is_enabled() && size > 0)
        {
            std::size_t processed = c->vector_add_into(x, y, size);

            size -= processed;
            x += processed;
            y += processed;
            metric += processed;
        }
    };

#if defined(PLATFORM_X86)
    vector_add_into(m_binary_avx2, m_field_metrics.m_avx2);
    vector_add_into(m_binary_sse2, m_field_metrics.m_sse2);
#elif defined(PLATFORM_NEON)
    vector_add_into(m_binary_neon, m_field_metrics.m_neon);
#endif
    vector_add_into(m_binary_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary4_math::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                        std::size_t size) const
{
    vector_add_into(x, y, size);
}

void binary4_math::vector_multiply_into(uint8_t* x, uint32_t constant,
                                        std::size_t size) const
{
    assert(x != nullptr);
    assert(size > 0);
    assert(constant < 16);

    const auto vector_multiply_into = [&](auto& c, abacus::metric& metric)
    {
        if (c.is_enabled() && size > 0)
        {
            std::size_t processed = c->vector_multiply_into(x, constant, size);

            size -= processed;
            x += processed;
            metric += processed;
        }
    };

#if defined(PLATFORM_X86)
    vector_multiply_into(m_binary4_avx2, m_field_metrics.m_avx2);
    vector_multiply_into(m_binary4_ssse3, m_field_metrics.m_ssse3);
#elif defined(PLATFORM_NEON)
    vector_multiply_into(m_binary4_neon, m_field_metrics.m_neon);
#endif
    vector_multiply_into(m_binary4_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary4_math::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                            uint32_t constant,
                                            std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert(constant < 16);

    const auto vector_multiply_add_into = [&](auto& c, abacus::metric& metric)
    {
        if (c.is_enabled() && size > 0)
        {
            std::size_t processed =
                c->vector_multiply_add_into(x, y, constant, size);

            size -= processed;
            x += processed;
            y += processed;
            metric += processed;
        }
    };

#if defined(PLATFORM_X86)
    vector_multiply_add_into(m_binary4_avx2, m_field_metrics.m_avx2);
    vector_multiply_add_into(m_binary4_ssse3, m_field_metrics.m_ssse3);
#elif defined(PLATFORM_NEON)
    vector_multiply_add_into(m_binary4_neon, m_field_metrics.m_neon);
#endif
    vector_multiply_add_into(m_binary4_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary4_math::vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                                 uint32_t constant,
                                                 std::size_t size) const
{
    vector_multiply_add_into(x, y, constant, size);
}

void binary4_math::vector_dot_product(uint8_t** x, const uint8_t** y,
                                      const uint8_t** constants,
                                      std::size_t size, std::size_t x_vectors,
                                      std::size_t y_vectors) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    // Consider optimizing some of these allocations away by using
    // a stack based allocator such as:
    // http://howardhinnant.github.io/stack_alloc.html
    //
    // We need to copy the pointers here since otherwise we would mess
    // with the callee's pointers.

    std::vector<uint8_t*> x_ptr(x, x + x_vectors);
    std::vector<const uint8_t*> y_ptr(y, y + y_vectors);

    const auto vector_dot_product = [&](auto& c, abacus::metric& metric)
    {
        if (c.is_enabled() && size > 0)
        {
            std::size_t processed =
                c->vector_dot_product(x_ptr.data(), y_ptr.data(), constants,
                                      size, x_vectors, y_vectors);

            size -= processed;

            for (auto& ptr : x_ptr)
            {
                ptr += processed;
            }

            for (auto& ptr : y_ptr)
            {
                ptr += processed;
            }

            metric += processed;
        }
    };

#if defined(PLATFORM_X86)
    vector_dot_product(m_binary4_avx2, m_field_metrics.m_avx2);
    vector_dot_product(m_binary4_ssse3, m_field_metrics.m_ssse3);
#elif defined(PLATFORM_NEON)
    vector_dot_product(m_binary4_neon, m_field_metrics.m_neon);
#endif
    vector_dot_product(m_binary4_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

std::size_t binary4_math::total_processed() const
{
    return detail::get_total_field_metrics(m_metrics);
}

auto binary4_math::metric_name(field_metric id) const -> std::string
{
    return m_metrics.metric_name(static_cast<uint64_t>(id));
}

auto binary4_math::metric_value(field_metric id) const -> uint64_t
{
    return m_metrics.metric_value(static_cast<uint64_t>(id));
}

void binary4_math::copy_metrics_storage(uint8_t* data) const
{
    m_metrics.copy_storage(data);
}

auto binary4_math::metrics_storage_bytes() const -> std::size_t
{
    return m_metrics.storage_bytes();
}

auto binary4_math::metrics_to_json() const -> std::string
{
    return m_metrics.to_json();
}

void binary4_math::reset_metrics()
{
    m_metrics.reset_metrics();
}

void binary4_math::reset_metric(field_metric id)
{
    m_metrics.reset_metric(static_cast<uint64_t>(id));
}

auto binary4_math::metrics() const -> std::size_t
{
    return m_metrics.max_metrics();
}

auto binary4_math::metric_id(std::size_t index) const -> field_metric
{
    return static_cast<field_metric>(index);
}

}
}
}
