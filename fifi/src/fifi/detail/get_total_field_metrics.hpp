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

#pragma once

#include "../version.hpp"
#include "field_metrics.hpp"

#include <cstdint>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
inline std::size_t get_total_field_metrics(const abacus::metrics& metrics)
{
    return metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_no_simd)) +
           metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_sse2)) +
           metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_ssse3)) +
           metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_sse42)) +
           metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_avx2)) +
           metrics.metric_value(
               static_cast<uint64_t>(field_metric::bytes_processed_neon));
}
}
}
}