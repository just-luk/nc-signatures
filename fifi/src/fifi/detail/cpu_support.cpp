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

#include "cpu_support.hpp"
#include "../version.hpp"

#include <cassert>

#include <cpuid/cpuinfo.hpp>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{

cpu_support::cpu_support()
{
    cpuid::cpuinfo cpuinfo;

    sse2 = cpuinfo.has_sse2();
    ssse3 = cpuinfo.has_ssse3();
    sse42 = cpuinfo.has_sse4_2();
    avx2 = cpuinfo.has_avx2();
    neon = cpuinfo.has_neon();
}

bool cpu_support::has(simd simd_type)
{
    switch (simd_type)
    {
    case simd::no_simd:
        return true;
    case simd::sse2:
        return sse2;
    case simd::ssse3:
        return ssse3;
    case simd::sse42:
        return sse42;
    case simd::avx2:
        return avx2;
    case simd::neon:
        return neon;
    default:
        assert(0 && "Unsupported SIMD option");
        return false;
    };
}

}
}
}