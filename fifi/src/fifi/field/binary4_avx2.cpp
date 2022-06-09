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

#include "binary4_avx2.hpp"

#include <cassert>
#include <platform/config.hpp>

// Include x86 intrinsics for GCC-compatible compilers on x86/x86_64
#if defined(PLATFORM_GCC_COMPATIBLE_X86)
#include <x86intrin.h>
#elif defined(PLATFORM_MSVC_X86)
#include <immintrin.h>
#endif

#include "get_value.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char binary4_avx2::name[];

finite_field binary4_avx2::field() const
{
    return fifi::finite_field::binary4;
}

binary4 binary4_avx2::field_info() const
{
    return m_field;
}

#ifdef PLATFORM_AVX2

std::size_t binary4_avx2::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                                   uint32_t constant,
                                                   std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert(constant < 16);

    // Initialize the look-up tables
    // Load the 32-byte row that contains pre-calculated multiplication
    // results with the constant
    __m256i table1 = _mm256_loadu_si256((const __m256i*)(m_low[constant]));

    // table2 contains the same results shifted left by 4 bits
    __m256i table2 = _mm256_loadu_si256((const __m256i*)(m_high[constant]));

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    __m256i* x_ptr = (__m256i*)x;
    const __m256i* y_ptr = (const __m256i*)y;

    // We loop 32 bytes at-a-time, so we calculate how many loops we need
    std::size_t simd_size = size / 32;
    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Multiply the src with the constant

        // Load the next 32-bytes of the source buffer
        __m256i ymm0 = _mm256_loadu_si256(y_ptr + i);
        // Apply mask1 to get the low-half of the data
        __m256i l = _mm256_and_si256(ymm0, mask1);
        // Perform 32 simultaneous table lookups to multiply the low-half
        l = _mm256_shuffle_epi8(table1, l);
        // Apply mask2 to get the high-half of the data
        __m256i h = _mm256_and_si256(ymm0, mask2);
        // Right shift the high-half by 4 bits to get values in [0,15]
        h = _mm256_srli_epi64(h, 4);
        // Perform table lookup with these indices to multiply the high-half
        h = _mm256_shuffle_epi8(table2, h);
        // Xor the high and low halves together to get the final result
        ymm0 = _mm256_xor_si256(h, l);

        // Add this product to the dest

        // Load the next 32-bytes of the destination buffer
        __m256i ymm1 = _mm256_loadu_si256(x_ptr + i);
        // Xor the multiplication result and the destination value
        ymm0 = _mm256_xor_si256(ymm0, ymm1);
        // Store the result in the destination buffer
        _mm256_storeu_si256(x_ptr + i, ymm0);
    }

    return simd_size * 32;
}

std::size_t binary4_avx2::vector_multiply_into(uint8_t* x, uint32_t constant,
                                               std::size_t size) const
{
    assert(x != nullptr);
    assert(size > 0U);
    assert(constant < 16U);

    // Initialize the look-up tables
    // Load the 32-byte row that contains pre-calculated multiplication
    // results with the constant
    __m256i table1 = _mm256_loadu_si256((const __m256i*)(m_low[constant]));

    // table2 contains the same results shifted left by 4 bits
    __m256i table2 = _mm256_loadu_si256((const __m256i*)(m_high[constant]));

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    __m256i* x_ptr = (__m256i*)x;

    // We loop 32 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 32;
    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 32-bytes of the destination buffer
        __m256i ymm0 = _mm256_loadu_si256(x_ptr + i);

        // Apply mask1 to get the low-half of the data
        __m256i l = _mm256_and_si256(ymm0, mask1);

        // Perform 32 simultaneous table lookups to multiply the low-half
        l = _mm256_shuffle_epi8(table1, l);

        // Apply mask2 to get the high-half of the data
        __m256i h = _mm256_and_si256(ymm0, mask2);

        // Right shift the high-half by 4 bits to get values in [0,15]
        h = _mm256_srli_epi64(h, 4);

        // Perform table lookup with these indices to multiply the high-half
        h = _mm256_shuffle_epi8(table2, h);

        // Xor the high and low halves together to get the final result
        ymm0 = _mm256_xor_si256(h, l);

        // Store the result in the destination buffer
        _mm256_storeu_si256(x_ptr + i, ymm0);
    }

    return simd_size * 32;
}

std::size_t binary4_avx2::vector_dot_product(uint8_t** x, const uint8_t** y,
                                             const uint8_t** constants,
                                             std::size_t size,
                                             std::size_t x_vectors,
                                             std::size_t y_vectors) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    std::size_t simd_size = (size / 32) * 32;

    if (simd_size == 0)
        return 0;

    for (std::size_t i = 0; i < x_vectors; ++i)
    {
        // Zero-initialize the destination buffer
        std::fill_n(x[i], size, 0);

        for (std::size_t j = 0; j < y_vectors; ++j)
        {
            // The vectors are multiplied and added into the result
            uint8_t constant = get_value(binary4(), constants[i], j);
            auto processed =
                vector_multiply_add_into(x[i], y[j], constant, size);
            assert(processed == simd_size);
            (void)processed;
        }
    }

    return simd_size;
}

bool binary4_avx2::is_compiled()
{
    return true;
}

#else

std::size_t binary4_avx2::vector_multiply_into(uint8_t*, uint32_t,
                                               std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary4_avx2::vector_multiply_add_into(uint8_t*, const uint8_t*,
                                                   uint32_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary4_avx2::vector_dot_product(uint8_t**, const uint8_t**,
                                             const uint8_t**, std::size_t,
                                             std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool binary4_avx2::is_compiled()
{
    return false;
}

#endif
}
}
}
