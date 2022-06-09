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

#include <cstdint>

#include "version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
/// The metrics for the different SIMD accelerations.
enum class field_metric : uint8_t
{
    /// Bytes that have been processed with No Acceleration
    bytes_processed_no_simd,
    /// Bytes that have been processed with SSE2 Acceleration
    bytes_processed_sse2,
    /// Bytes that have been processed with SSSE3 Acceleration
    bytes_processed_ssse3,
    /// Bytes that have been processed with SSE4.2 Acceleration
    bytes_processed_sse42,
    /// Bytes that have been processed with AVX2 Acceleration
    bytes_processed_avx2,
    /// Bytes that have been processed with NEON Acceleration
    bytes_processed_neon,
    /// SSE2 Acceleration
    has_sse2,
    /// SSSE3 Acceleration
    has_ssse3,
    /// SSE4.2 Acceleration
    has_sse42,
    /// AVX2 Acceleration
    has_avx2,
    /// NEON Acceleration
    has_neon,
    /// The degree of the field used (1 for binary, 4 for binary4, etc..)
    field_degree,
    /// The field polynomial used
    field_polynomial
};
}
}
