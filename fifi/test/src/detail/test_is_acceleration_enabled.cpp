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

#include <fifi/detail/is_acceleration_enabled.hpp>
#include <fifi/simd.hpp>

#include <cpuid/cpuinfo.hpp>

#include <gtest/gtest.h>

namespace
{
template <fifi::simd Simd>
struct enabled
{
    static constexpr fifi::simd simd_type = Simd;

    static bool is_compiled()
    {
        return true;
    }
};

template <fifi::simd Simd>
struct disabled
{
    static constexpr fifi::simd simd_type = Simd;
    static bool is_compiled()
    {
        return false;
    }
};
}

TEST(test_is_acceleration_enabled, check)
{
    cpuid::cpuinfo c;

    using namespace fifi;
    using namespace fifi::detail;

    EXPECT_EQ(c.has_sse2(), is_acceleration_enabled<enabled<simd::sse2>>());
    EXPECT_EQ(c.has_ssse3(), is_acceleration_enabled<enabled<simd::ssse3>>());
    EXPECT_EQ(c.has_avx2(), is_acceleration_enabled<enabled<simd::avx2>>());
    EXPECT_EQ(c.has_neon(), is_acceleration_enabled<enabled<simd::neon>>());

    EXPECT_FALSE(is_acceleration_enabled<disabled<simd::sse2>>());
    EXPECT_FALSE(is_acceleration_enabled<disabled<simd::ssse3>>());
    EXPECT_FALSE(is_acceleration_enabled<disabled<simd::avx2>>());
    EXPECT_FALSE(is_acceleration_enabled<disabled<simd::neon>>());
}
