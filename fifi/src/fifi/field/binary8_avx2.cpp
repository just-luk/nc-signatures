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

#include "binary8_avx2.hpp"

#include "../detail/array_data.hpp"
#include "../detail/simd_allocator.hpp"

#include <cassert>
#include <vector>

#include <platform/config.hpp>

// Include x86 intrinsics for GCC-compatible compilers on x86/x86_64
#if defined(PLATFORM_GCC_COMPATIBLE_X86)
#include <x86intrin.h>
#elif defined(PLATFORM_MSVC_X86)
#include <immintrin.h>
#include <malloc.h>
#endif

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char binary8_avx2::name[];

finite_field binary8_avx2::field() const
{
    return fifi::finite_field::binary8;
}

binary8 binary8_avx2::field_info() const
{
    return m_field;
}

#ifdef PLATFORM_AVX2

std::size_t binary8_avx2::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                                   uint32_t constant,
                                                   std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert(constant < 256);

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

    // We loop 32 bytes at-a-time so we calculate how many
    // loops we need
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

std::size_t binary8_avx2::vector_multiply_into(uint8_t* x, uint32_t constant,
                                               std::size_t size) const
{
    assert(x != nullptr);
    assert(size > 0U);
    assert(constant < 256U);

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

std::size_t binary8_avx2::vector_dot_product(uint8_t** x, const uint8_t** y,
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

    // unoptimized is the number of vectors that cannot be calculated with the
    // optimized functions that operates on 4 vectors at the same time
    std::size_t unoptimized = x_vectors % 4;
    std::size_t optimized = x_vectors - unoptimized;

    // Use the optimized functions for calculating 'optimized' x vectors
    if (optimized > 0)
    {
        vector4_dot_product(x, y, constants, simd_size, optimized, y_vectors);
    }

    // The rest of the destination vectors will be calculated with the
    // other functions that are optimized for 1, 2 or 3 vectors
    switch (unoptimized)
    {
    case 3:
        vector3_dot_product(&x[optimized], y, &constants[optimized], simd_size,
                            unoptimized, y_vectors);
        break;
    case 2:
        vector2_dot_product(&x[optimized], y, &constants[optimized], simd_size,
                            unoptimized, y_vectors);
        break;
    case 1:
        vector1_dot_product(&x[optimized], y, &constants[optimized], simd_size,
                            unoptimized, y_vectors);
        break;
    case 0:
        break;
    }

    return simd_size;
}

namespace
{
struct alignas(32) packed
{
    __m256i low;
    __m256i high;
};

template <class T>
using simd_vector = std::vector<T, detail::simd_allocator<T>>;

}

template <std::size_t UnrollFactor>
static inline simd_vector<packed>
pack_constants(const detail::table_data<uint8_t, 256, 32>& low,
               const detail::table_data<uint8_t, 256, 32>& high,
               const uint8_t** constants, std::size_t constant_vectors,
               std::size_t vector_elements)
{
    assert(constant_vectors % UnrollFactor == 0);

    // Pre-load the multiplication segments for all coefficients to improve
    // the cache coherence during the actual multiplications.
    // It is faster to access the packed constants sequentially compared to
    // jumping to various indices in the low and high tables.
    simd_vector<packed> packed_constants(constant_vectors * vector_elements);
    __m256i* pack_ptr = (__m256i*)(packed_constants.data());

    __m256i table;
    for (std::size_t i = 0; i < constant_vectors; i += UnrollFactor)
    {
        for (std::size_t j = 0; j < vector_elements; ++j)
        {
            for (std::size_t k = 0; k < UnrollFactor; ++k)
            {
                uint8_t constant = constants[i + k][j];

                table = _mm256_loadu_si256((const __m256i*)(low[constant]));
                _mm256_storeu_si256(pack_ptr++, table);
                table = _mm256_loadu_si256((const __m256i*)(high[constant]));
                _mm256_storeu_si256(pack_ptr++, table);
            }
        }
    }

    return packed_constants;
}

static inline void _multiply_helper(__m256i& low, __m256i& high, packed*& pack,
                                    __m256i& sum)
{
    // Perform 32 simultaneous table lookups to multiply the low half
    __m256i l = _mm256_shuffle_epi8(pack->low, low);
    // Perform table lookups to multiply the high half
    __m256i h = _mm256_shuffle_epi8(pack->high, high);
    // Xor the high and low halves together to get the product
    __m256i product = _mm256_xor_si256(h, l);
    // Add the product to the partial sum
    sum = _mm256_xor_si256(sum, product);

    pack++;
}

std::size_t binary8_avx2::vector1_dot_product(uint8_t** x, const uint8_t** y,
                                              const uint8_t** constants,
                                              std::size_t size,
                                              std::size_t x_vectors,
                                              std::size_t y_vectors) const
{
    assert(x != 0);
    assert(y != 0);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    simd_vector<packed> packed_constants =
        pack_constants<1>(m_low, m_high, constants, x_vectors, y_vectors);

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    // We loop 32 bytes at-a-time, so we calculate how many loops we need
    std::size_t simd_size = size / 32;

    // Calculate the dot products
    for (std::size_t i = 0; i < x_vectors; i += 1)
    {
        // Get the pointer to the i'th destination vector
        __m256i* x_ptr = (__m256i*)x[i];

        for (std::size_t m = 0; m < simd_size; ++m)
        {
            // Get the offset to the packed constants
            packed* p_ptr = &packed_constants[i * y_vectors];

            // Zero-initialize the partial sums
            __m256i sum = _mm256_setzero_si256();

            // Go though all the source vectors
            for (std::size_t k = 0; k < y_vectors; ++k)
            {
                const __m256i* y_ptr = (const __m256i*)y[k];
                // Load the next 32 bytes of the source buffer
                __m256i src = _mm256_loadu_si256(y_ptr + m);
                // Apply mask1 to get the low-half of the data
                __m256i low = _mm256_and_si256(src, mask1);
                // Apply mask2 to get the high-half of the data
                __m256i high = _mm256_and_si256(src, mask2);
                // Right shift the high-half by 4 bits to get values in [0,15]
                high = _mm256_srli_epi64(high, 4);

                // Add the calculated products to the partial sums
                _multiply_helper(low, high, p_ptr, sum);
            }

            // Store the results in the destination buffers
            _mm256_storeu_si256(x_ptr + m, sum);
        }
    }
    return simd_size;
}

std::size_t binary8_avx2::vector2_dot_product(uint8_t** x, const uint8_t** y,
                                              const uint8_t** constants,
                                              std::size_t size,
                                              std::size_t x_vectors,
                                              std::size_t y_vectors) const
{
    assert(x != 0);
    assert(y != 0);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    simd_vector<packed> packed_constants =
        pack_constants<2>(m_low, m_high, constants, x_vectors, y_vectors);

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    // We loop 32 bytes at-a-time, so we calculate how many loops we need
    std::size_t simd_size = size / 32;

    // Calculate the dot products
    for (std::size_t i = 0; i < x_vectors; i += 2)
    {
        // Get the pointer to the i'th destination vector
        __m256i* x_ptr[2];
        for (std::size_t h = 0; h < 2; ++h)
            x_ptr[h] = (__m256i*)x[i + h];

        for (std::size_t m = 0; m < simd_size; ++m)
        {
            // Get the offset to the packed constants
            packed* p_ptr = &packed_constants[i * y_vectors];

            // Zero-initialize the partial sums
            __m256i sum[2];
            sum[0] = _mm256_setzero_si256();
            sum[1] = _mm256_setzero_si256();

            // Go though all the source vectors
            for (std::size_t k = 0; k < y_vectors; ++k)
            {
                const __m256i* y_ptr = (const __m256i*)y[k];
                // Load the next 32 bytes of the source buffer
                __m256i src = _mm256_loadu_si256(y_ptr + m);
                // Apply mask1 to get the low-half of the data
                __m256i low = _mm256_and_si256(src, mask1);
                // Apply mask2 to get the high-half of the data
                __m256i high = _mm256_and_si256(src, mask2);
                // Right shift the high-half by 4 bits to get values in [0,15]
                high = _mm256_srli_epi64(high, 4);

                // Add the calculated products to the partial sums
                _multiply_helper(low, high, p_ptr, sum[0]);
                _multiply_helper(low, high, p_ptr, sum[1]);
            }

            // Store the results in the destination buffers
            _mm256_storeu_si256(x_ptr[0] + m, sum[0]);
            _mm256_storeu_si256(x_ptr[1] + m, sum[1]);
        }
    }
    return simd_size;
}

std::size_t binary8_avx2::vector3_dot_product(uint8_t** x, const uint8_t** y,
                                              const uint8_t** constants,
                                              std::size_t size,
                                              std::size_t x_vectors,
                                              std::size_t y_vectors) const
{
    assert(x != 0);
    assert(y != 0);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    simd_vector<packed> packed_constants =
        pack_constants<3>(m_low, m_high, constants, x_vectors, y_vectors);

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    // We loop 32 bytes at-a-time, so we calculate how many loops we need
    std::size_t simd_size = size / 32;

    // Calculate the dot products
    for (std::size_t i = 0; i < x_vectors; i += 3)
    {
        // Get the pointer to the i'th destination vector
        __m256i* x_ptr[3];
        for (std::size_t h = 0; h < 3; ++h)
            x_ptr[h] = (__m256i*)x[i + h];

        for (std::size_t m = 0; m < simd_size; ++m)
        {
            // Get the offset to the packed constants
            packed* p_ptr = &packed_constants[i * y_vectors];

            // Zero-initialize the partial sums
            __m256i sum[3];
            sum[0] = _mm256_setzero_si256();
            sum[1] = _mm256_setzero_si256();
            sum[2] = _mm256_setzero_si256();

            // Go though all the source vectors
            for (std::size_t k = 0; k < y_vectors; ++k)
            {
                const __m256i* y_ptr = (const __m256i*)y[k];
                // Load the next 32 bytes of the source buffer
                __m256i src = _mm256_loadu_si256(y_ptr + m);
                // Apply mask1 to get the low-half of the data
                __m256i low = _mm256_and_si256(src, mask1);
                // Apply mask2 to get the high-half of the data
                __m256i high = _mm256_and_si256(src, mask2);
                // Right shift the high-half by 4 bits to get values in [0,15]
                high = _mm256_srli_epi64(high, 4);

                // Add the calculated products to the partial sums
                _multiply_helper(low, high, p_ptr, sum[0]);
                _multiply_helper(low, high, p_ptr, sum[1]);
                _multiply_helper(low, high, p_ptr, sum[2]);
            }

            // Store the results in the destination buffers
            _mm256_storeu_si256(x_ptr[0] + m, sum[0]);
            _mm256_storeu_si256(x_ptr[1] + m, sum[1]);
            _mm256_storeu_si256(x_ptr[2] + m, sum[2]);
        }
    }
    return simd_size;
}

std::size_t binary8_avx2::vector4_dot_product(uint8_t** x, const uint8_t** y,
                                              const uint8_t** constants,
                                              std::size_t size,
                                              std::size_t x_vectors,
                                              std::size_t y_vectors) const
{
    assert(x != 0);
    assert(y != 0);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    simd_vector<packed> packed_constants =
        pack_constants<4>(m_low, m_high, constants, x_vectors, y_vectors);

    // Create low and high bitmasks by replicating the mask values 32 times
    __m256i mask1 = _mm256_set1_epi8((char)0x0f);
    __m256i mask2 = _mm256_set1_epi8((char)0xf0);

    // We loop 32 bytes at-a-time, so we calculate how many loops we need
    std::size_t simd_size = size / 32;

    // Calculate the dot products
    for (std::size_t i = 0; i < x_vectors; i += 4)
    {
        // Get the pointer to the i'th destination vector
        __m256i* x_ptr[4];
        for (std::size_t h = 0; h < 4; ++h)
            x_ptr[h] = (__m256i*)x[i + h];

        for (std::size_t m = 0; m < simd_size; ++m)
        {
            // Get the offset to the packed constants
            packed* p_ptr = &packed_constants[i * y_vectors];

            // Zero-initialize the partial sums
            __m256i sum[4];
            sum[0] = _mm256_setzero_si256();
            sum[1] = _mm256_setzero_si256();
            sum[2] = _mm256_setzero_si256();
            sum[3] = _mm256_setzero_si256();

            // Go though all the source vectors
            for (std::size_t k = 0; k < y_vectors; ++k)
            {
                const __m256i* y_ptr = (const __m256i*)y[k];
                // Load the next 32 bytes of the source buffer
                __m256i src = _mm256_loadu_si256(y_ptr + m);
                // Apply mask1 to get the low-half of the data
                __m256i low = _mm256_and_si256(src, mask1);
                // Apply mask2 to get the high-half of the data
                __m256i high = _mm256_and_si256(src, mask2);
                // Right shift the high-half by 4 bits to get values in [0,15]
                high = _mm256_srli_epi64(high, 4);

                // Add the calculated products to the partial sums
                _multiply_helper(low, high, p_ptr, sum[0]);
                _multiply_helper(low, high, p_ptr, sum[1]);
                _multiply_helper(low, high, p_ptr, sum[2]);
                _multiply_helper(low, high, p_ptr, sum[3]);
            }

            // Store the results in the destination buffers
            _mm256_storeu_si256(x_ptr[0] + m, sum[0]);
            _mm256_storeu_si256(x_ptr[1] + m, sum[1]);
            _mm256_storeu_si256(x_ptr[2] + m, sum[2]);
            _mm256_storeu_si256(x_ptr[3] + m, sum[3]);
        }
    }
    return simd_size;
}

bool binary8_avx2::is_compiled()
{
    return true;
}

#else

std::size_t binary8_avx2::vector_multiply_add_into(uint8_t*, const uint8_t*,
                                                   uint32_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector_multiply_into(uint8_t*, uint32_t,
                                               std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector_dot_product(uint8_t**, const uint8_t**,
                                             const uint8_t**, std::size_t,
                                             std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector1_dot_product(uint8_t**, const uint8_t**,
                                              const uint8_t**, std::size_t,
                                              std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector2_dot_product(uint8_t**, const uint8_t**,
                                              const uint8_t**, std::size_t,
                                              std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector3_dot_product(uint8_t**, const uint8_t**,
                                              const uint8_t**, std::size_t,
                                              std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary8_avx2::vector4_dot_product(uint8_t**, const uint8_t**,
                                              const uint8_t**, std::size_t,
                                              std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool binary8_avx2::is_compiled()
{
    return false;
}
#endif
}
}
}
