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

#include <fifi/detail/field_metrics.hpp>
#include <fifi/detail/get_total_field_metrics.hpp>
#include <fifi/field_metric.hpp>

#include <gtest/gtest.h>

TEST(test_get_all_field_metrics, api)
{
    abacus::metrics metrics(6, 32, "field metrics");

    fifi::detail::field_metrics field_metric;

    field_metric.m_no_simd = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_no_simd),
        "no_simd");
    field_metric.m_sse2 = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_sse2),
        "SSE2");
    field_metric.m_ssse3 = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_ssse3),
        "SSSE3");
    field_metric.m_sse42 = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_sse42),
        "SSE4.2");
    field_metric.m_avx2 = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_avx2),
        "AVX2");
    field_metric.m_neon = metrics.initialize_metric(
        static_cast<uint64_t>(fifi::field_metric::bytes_processed_neon),
        "Neon");

    EXPECT_EQ(0U, fifi::detail::get_total_field_metrics(metrics));

    field_metric.m_no_simd = 1;
    field_metric.m_sse2 = 2;
    field_metric.m_ssse3 = 3;
    field_metric.m_sse42 = 4;
    field_metric.m_avx2 = 5;
    field_metric.m_neon = 6;

    EXPECT_EQ(21U, fifi::detail::get_total_field_metrics(metrics));
}
