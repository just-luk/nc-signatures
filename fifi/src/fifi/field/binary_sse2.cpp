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

#include "binary_sse2.hpp"

#include <cassert>
#include <platform/config.hpp>

// Include x86 intrinsics for GCC-compatible compilers on x86/x86_64
#if defined(PLATFORM_GCC_COMPATIBLE_X86)
#include <x86intrin.h>
#elif defined(PLATFORM_MSVC_X86)
#include <immintrin.h>
#endif

#include <cpuid/cpuinfo.hpp>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
constexpr char binary_sse2::name[];

finite_field binary_sse2::field() const
{
    return fifi::finite_field::binary;
}

binary binary_sse2::field_info() const
{
    return binary();
}

#ifdef PLATFORM_SSE2

std::size_t binary_sse2::vector_add_into(uint8_t* x, const uint8_t* y,
                                         std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    __m128i* x_ptr = (__m128i*)x;
    const __m128i* y_ptr = (const __m128i*)y;

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16;

    std::size_t optimized = simd_size / 8;
    std::size_t unoptimized = simd_size - (optimized * 8);

    const auto xor_helper = [&]()
    {
        // Load the next 16-bytes of the destination and source buffers
        __m128i xmm0 = _mm_loadu_si128(x_ptr);
        __m128i xmm1 = _mm_loadu_si128(y_ptr);

        // Xor these values together
        xmm0 = _mm_xor_si128(xmm0, xmm1);

        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr, xmm0);

        ++x_ptr;
        ++y_ptr;
    };

    for (std::size_t i = 0; i < optimized; ++i)
    {
        xor_helper();
        xor_helper();
        xor_helper();
        xor_helper();
        xor_helper();
        xor_helper();
        xor_helper();
        xor_helper();
    }

    switch (unoptimized)
    {
    case 7:
        xor_helper();
    // fall through
    case 6:
        xor_helper();
    // fall through
    case 5:
        xor_helper();
    // fall through
    case 4:
        xor_helper();
    // fall through
    case 3:
        xor_helper();
    // fall through
    case 2:
        xor_helper();
    // fall through
    case 1:
        xor_helper();
    // fall through
    default:
        break;
    }

    return simd_size * 16;
}

bool binary_sse2::is_compiled()
{
    return true;
}

#else

std::size_t binary_sse2::vector_add_into(uint8_t*, const uint8_t*,
                                         std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool binary_sse2::is_compiled()
{
    return false;
}

#endif
}
}
}
