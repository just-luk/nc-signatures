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

#include <fifi/detail/cpu_support.hpp>
#include <fifi/simd.hpp>

#include <cpuid/cpuinfo.hpp>

#include <gtest/gtest.h>

TEST(test_cpu_support, check)
{
    cpuid::cpuinfo cpuid;

    using namespace fifi;

    detail::cpu_support cpu;

    EXPECT_EQ(cpuid.has_sse2(), cpu.sse2);
    EXPECT_EQ(cpuid.has_ssse3(), cpu.ssse3);
    EXPECT_EQ(cpuid.has_sse4_2(), cpu.sse42);
    EXPECT_EQ(cpuid.has_avx2(), cpu.avx2);
    EXPECT_EQ(cpuid.has_neon(), cpu.neon);

    EXPECT_TRUE(cpu.has(simd::no_simd));
    EXPECT_EQ(cpuid.has_sse2(), cpu.has(simd::sse2));
    EXPECT_EQ(cpuid.has_ssse3(), cpu.has(simd::ssse3));
    EXPECT_EQ(cpuid.has_sse4_2(), cpu.has(simd::sse42));
    EXPECT_EQ(cpuid.has_avx2(), cpu.has(simd::avx2));
    EXPECT_EQ(cpuid.has_neon(), cpu.has(simd::neon));
}
