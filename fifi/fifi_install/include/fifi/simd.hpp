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
/// The SIMD acceleration
enum class simd : uint8_t
{
    /// No Acceleration
    no_simd,
    /// SSE2 Acceleration
    sse2,
    /// SSSE3 Acceleration
    ssse3,
    /// SSE4.2 Acceleration
    sse42,
    /// AVX2 Acceleration
    avx2,
    /// NEON Acceleration
    neon
};
}
}
