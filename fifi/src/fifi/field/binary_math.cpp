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

#include <bourne/json.hpp>
#include <cpuid/cpuinfo.hpp>
#include <platform/config.hpp>

#include <cassert>

#include "../detail/cpu_support.hpp"
#include "../detail/get_total_field_metrics.hpp"

#include "binary_math.hpp"
#include "bytes_to_elements.hpp"
#include "elements_to_bytes.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

// Default math implementation
static constexpr field::binary_backend default_binary{};

/// Initialize cpu support
static const detail::cpu_support cpu;

binary_math::binary_math() : binary_math::binary_math(&default_binary, cpu)
{
}

binary_math::binary_math(const binary_backend* backend) :
    binary_math::binary_math(backend, cpu)
{
}

binary_math::binary_math(const binary_backend* backend,
                         const detail::cpu_support& cpu) :
    m_metrics(13, 32, "Field")
{
    set_backend(backend, cpu);
    m_field_metrics.m_no_simd = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_no_simd),
        "No SIMD");
    m_field_metrics.m_sse2 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_sse2),
        "Bytes Processed SSE2");
    m_field_metrics.m_ssse3 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_ssse3),
        "Bytes Processed SSSE3");
    m_field_metrics.m_sse42 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_sse42),
        "Bytes Processed SSE4.2");
    m_field_metrics.m_avx2 = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_avx2),
        "Bytes Processed AVX2");
    m_field_metrics.m_neon = m_metrics.initialize_metric(
        static_cast<uint64_t>(field_metric::bytes_processed_neon),
        "Bytes Processed Neon");
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

void binary_math::set_backend(const binary_backend* backend)
{
    set_backend(backend, cpu);
}

void binary_math::set_backend(const binary_backend* backend,
                              const detail::cpu_support& cpu)
{
    assert(backend != nullptr);
    m_backend = backend;
    m_binary_avx2 = {&m_backend->m_binary_avx2, cpu.avx2};
    m_binary_sse2 = {&m_backend->m_binary_sse2, cpu.sse2};
    m_binary_neon = {&m_backend->m_binary_neon, cpu.neon};
    m_binary_basic = {&m_backend->m_binary_basic, true};
}

finite_field binary_math::field() const
{
    return fifi::finite_field::binary;
}

binary binary_math::field_info() const
{
    return m_backend->m_field;
}

uint32_t binary_math::add(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary_basic->add(a, b);
}

uint32_t binary_math::subtract(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary_basic->subtract(a, b);
}

uint32_t binary_math::multiply(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary_basic->multiply(a, b);
}

uint32_t binary_math::divide(uint32_t a, uint32_t b) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary_basic->divide(a, b);
}

uint32_t binary_math::invert(uint32_t a) const
{
    m_field_metrics.m_no_simd += 1;
    return m_binary_basic->invert(a);
}

void binary_math::vector_add_into(uint8_t* x, const uint8_t* y,
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

void binary_math::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                       std::size_t size) const
{
    vector_add_into(x, y, size);
}

void binary_math::vector_multiply_into(uint8_t* x, uint32_t constant,
                                       std::size_t size) const
{
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

    vector_multiply_into(m_binary_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary_math::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                           uint32_t constant,
                                           std::size_t size) const
{
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

    vector_multiply_add_into(m_binary_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

void binary_math::vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                                uint32_t constant,
                                                std::size_t size) const
{
    vector_multiply_add_into(x, y, constant, size);
}

void binary_math::vector_dot_product(uint8_t** x, const uint8_t** y,
                                     const uint8_t** constants,
                                     std::size_t size, std::size_t x_vectors,
                                     std::size_t y_vectors) const
{
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

    vector_dot_product(m_binary_basic, m_field_metrics.m_no_simd);

    assert(size == 0);
}

std::size_t binary_math::total_processed() const
{
    return detail::get_total_field_metrics(m_metrics);
}

auto binary_math::metric_name(field_metric id) const -> std::string
{
    return m_metrics.metric_name(static_cast<uint64_t>(id));
}

auto binary_math::metric_value(field_metric id) const -> uint64_t
{
    return m_metrics.metric_value(static_cast<uint64_t>(id));
}

void binary_math::copy_metrics_storage(uint8_t* data) const
{
    m_metrics.copy_storage(data);
}

auto binary_math::metrics_storage_bytes() const -> std::size_t
{
    return m_metrics.storage_bytes();
}

auto binary_math::metrics_to_json() const -> std::string
{
    return m_metrics.to_json();
}

void binary_math::reset_metrics()
{
    m_metrics.reset_metrics();
}

void binary_math::reset_metric(field_metric id)
{
    m_metrics.reset_metric(static_cast<uint64_t>(id));
}

auto binary_math::metrics() const -> std::size_t
{
    return m_metrics.max_metrics();
}

auto binary_math::metric_id(std::size_t index) const -> field_metric
{
    return static_cast<field_metric>(index);
}

}
}
}
