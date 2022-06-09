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

#include "binary16_math.hpp"
#include "bytes_to_elements.hpp"
#include "elements_to_bytes.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

#include "../detail/cpu_support.hpp"
#include "../detail/get_total_field_metrics.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

// Default backend implementation
static constexpr field::binary16_backend default_binary16{field::binary16()};

/// Initialize cpu support
static const detail::cpu_support cpu;

binary16_math::binary16_math() : binary16_math::binary16_math(&default_binary16)
{
}

binary16_math::binary16_math(const binary16_backend* backend) :
    binary16_math::binary16_math(backend, cpu)
{
}

binary16_math::binary16_math(const binary16_backend* backend,
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

    m_field_metrics.m_field_degree = backend->m_field.degree;
    m_field_metrics.m_field_polynomial = backend->m_field.prime();
}

void binary16_math::set_backend(const binary16_backend* backend)
{
    set_backend(backend, cpu);
}

void binary16_math::set_backend(const binary16_backend* backend,
                                const detail::cpu_support& cpu)
{
    assert(backend != nullptr);
    m_backend = backend;
    m_binary_avx2 = {&m_backend->m_binary_avx2, cpu.avx2};
    m_binary_sse2 = {&m_backend->m_binary_sse2, cpu.sse2};
    m_binary_basic = {&m_backend->m_binary_basic, true};
    m_binary16_basic = {&m_backend->m_binary16_basic, true};

    auto check_prime = [](auto a, auto prime)
    { return !a.is_enabled() || a->field_info().prime() == prime; };

    assert(check_prime(m_binary16_basic, field_info().prime()));

    // Silence warnings if compiling without asserts
    (void)check_prime;
}

finite_field binary16_math::field() const
{
    return fifi::finite_field::binary16;
}

binary16 binary16_math::field_info() const
{
    assert(m_backend != nullptr);

    return m_backend->m_field;
}

uint32_t binary16_math::add(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 2;
    return m_binary16_basic->add(a, b);
}

uint32_t binary16_math::subtract(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 2;
    return m_binary16_basic->subtract(a, b);
}

uint32_t binary16_math::multiply(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 2;
    return m_binary16_basic->multiply(a, b);
}

uint32_t binary16_math::divide(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 2;
    return m_binary16_basic->divide(a, b);
}

uint32_t binary16_math::invert(uint32_t a) const
{
    m_field_metrics.m_no_simd += 2;
    return m_binary16_basic->invert(a);
}

void binary16_math::vector_add_into(uint8_t* x, const uint8_t* y,
                                    std::size_t size) const
{
    assert((size % binary16::granularity) == 0);

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
#endif
    vector_add_into(m_binary_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary16_math::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                         std::size_t size) const
{
    assert((size % binary16::granularity) == 0);
    vector_add_into(x, y, size);
}

void binary16_math::vector_multiply_into(uint8_t* x, uint32_t constant,
                                         std::size_t size) const
{
    assert((size % binary16::granularity) == 0);

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

    vector_multiply_into(m_binary16_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary16_math::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                             uint32_t constant,
                                             std::size_t size) const
{
    assert((size % binary16::granularity) == 0);

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

    vector_multiply_add_into(m_binary16_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary16_math::vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                                  uint32_t constant,
                                                  std::size_t size) const
{
    assert((size % binary16::granularity) == 0);
    vector_multiply_add_into(x, y, constant, size);
}

void binary16_math::vector_dot_product(uint8_t** x, const uint8_t** y,
                                       const uint8_t** constants,
                                       std::size_t size, std::size_t x_vectors,
                                       std::size_t y_vectors) const
{
    assert((size % binary16::granularity) == 0);

    const auto vector_dot_product = [&](auto& c, abacus::metric& metric)
    {
        if (c.is_enabled() && size > 0)
        {
            std::size_t processed = c->vector_dot_product(x, y, constants, size,
                                                          x_vectors, y_vectors);

            size -= processed;
            x += processed;
            y += processed;
            metric += processed;
        }
    };

    vector_dot_product(m_binary16_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

uint64_t binary16_math::total_processed() const
{
    return detail::get_total_field_metrics(m_metrics);
}

auto binary16_math::metric_name(field_metric id) const -> std::string
{
    return m_metrics.metric_name(static_cast<uint64_t>(id));
}

auto binary16_math::metric_value(field_metric id) const -> uint64_t
{
    return m_metrics.metric_value(static_cast<uint64_t>(id));
}

void binary16_math::copy_metrics_storage(uint8_t* data) const
{
    m_metrics.copy_storage(data);
}

auto binary16_math::metrics_storage_bytes() const -> std::size_t
{
    return m_metrics.storage_bytes();
}

auto binary16_math::metrics_to_json() const -> std::string
{
    return m_metrics.to_json();
}

void binary16_math::reset_metrics()
{
    m_metrics.reset_metrics();
}

void binary16_math::reset_metric(field_metric id)
{
    m_metrics.reset_metric(static_cast<uint64_t>(id));
}

auto binary16_math::metrics() const -> std::size_t
{
    return m_metrics.max_metrics();
}

auto binary16_math::metric_id(std::size_t index) const -> field_metric
{
    return static_cast<field_metric>(index);
}

}
}
}
