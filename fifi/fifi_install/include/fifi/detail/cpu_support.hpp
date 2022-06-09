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

#include "../simd.hpp"
#include "../version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{

struct cpu_support
{
    /// Constructor
    cpu_support();

    bool has(simd simd_type);

    /// CPU has support of SSE2
    bool sse2;
    /// CPU has support of SSSE3
    bool ssse3;
    /// CPU has support of SSE42
    bool sse42;
    /// CPU has support of AVX2
    bool avx2;
    /// CPU has support of NEON
    bool neon;
};
}
}
}