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

#include "prime2325_sse42.hpp"
#include "../online/add.hpp"
#include "../online/divide.hpp"
#include "../online/invert.hpp"
#include "../online/multiply.hpp"
#include "../online/subtract.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>

#include <platform/config.hpp>

// Include x86 intrinsics for GCC-compatible compilers on x86/x86_64
#if defined(PLATFORM_GCC_COMPATIBLE_X86)
#include <x86intrin.h>
#elif defined(PLATFORM_MSVC_X86)
#include <immintrin.h>
#endif

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char prime2325_sse42::name[];

finite_field prime2325_sse42::field() const
{
    return fifi::finite_field::prime2325;
}

prime2325 prime2325_sse42::field_info() const
{
    return prime2325();
}

#ifdef PLATFORM_SSE42

namespace
{
static inline void add_helper(__m128i& a, const __m128i& b, const __m128i& mask,
                              const __m128i& five, const __m128i& prime,
                              const __m128i& xprime)
{
    // Add these values and store the sum in 'a'
    a = _mm_add_epi32(a, b);

    // Create overflow mask
    // An overflow occurred if b is greater than the sum (b > sum)
    // Note: There is no unsigned comparison, so we XOR the
    // arguments with 0x80000000 to simulate a signed comparison
    __m128i overflow =
        _mm_cmpgt_epi32(_mm_xor_si128(mask, b), _mm_xor_si128(mask, a));

    // Add 5 if an overflow occurred
    a = _mm_add_epi32(a, _mm_and_si128(overflow, five));

    // The final sum must be smaller than the prime
    // We create a mask whose elements will be non-zero when the
    // prime is greater than the sum (prime > sum)
    __m128i primemask = _mm_cmpgt_epi32(xprime, _mm_xor_si128(mask, a));

    // Subtract the prime based on the inverse of the mask
    a = _mm_sub_epi32(a, _mm_andnot_si128(primemask, prime));
}

static inline void multiply_helper(__m128i& prod, const __m128i& lowmask,
                                   const __m128i& five, const __m128i& prime64)
{
    // Store the low 32 bits in l1
    __m128i l1 = _mm_and_si128(prod, lowmask);
    // Shift the product right by 32 bits and multiply by 5
    prod = _mm_srli_epi64(prod, 32);
    prod = _mm_mul_epu32(prod, five);

    // Store the low 32 bits in l2
    __m128i l2 = _mm_and_si128(prod, lowmask);
    // Shift the product right by 32 bits and multiply by 5
    prod = _mm_srli_epi64(prod, 32);
    prod = _mm_mul_epu32(prod, five);

    // Add l1 and l2
    prod = _mm_add_epi64(prod, l1);
    prod = _mm_add_epi64(prod, l2);

    // The final product must be smaller than the prime
    // We create a mask whose elements will be non-zero when the
    // prime is greater than prod (prime > product)
    // This can be done as a signed comparison, since the value of
    // the product cannot be higher than 2 * 2^32
    __m128i primemask = _mm_cmpgt_epi64(prime64, prod);

    // Subtract the prime based on the inverse of the mask
    prod = _mm_sub_epi64(prod, _mm_andnot_si128(primemask, prime64));
}
}

std::size_t prime2325_sse42::vector_add_into(uint8_t* x, const uint8_t* y,
                                             std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16U;

    __m128i mask = _mm_set1_epi32(0x80000000);
    __m128i five = _mm_set1_epi32(5);
    __m128i prime = _mm_set1_epi32(prime2325().prime());
    // XOR-ed prime that will be used in signed comparisons
    __m128i xprime = _mm_set1_epi32(prime2325().prime() ^ 0x80000000);

    __m128i* x_ptr = (__m128i*)x;
    const __m128i* y_ptr = (const __m128i*)y;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 16-bytes of the destination and source buffers
        __m128i x_value = _mm_loadu_si128(x_ptr + i);
        __m128i y_value = _mm_loadu_si128(y_ptr + i);

        add_helper(x_value, y_value, mask, five, prime, xprime);

        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr + i, x_value);
    }

    return simd_size * 16U;
}

std::size_t prime2325_sse42::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                                  std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16;

    __m128i mask = _mm_set1_epi32(0x80000000);
    __m128i five = _mm_set1_epi32(5);

    __m128i* x_ptr = (__m128i*)x;
    const __m128i* y_ptr = (const __m128i*)y;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 16-bytes of the destination and source buffers
        __m128i x_value = _mm_loadu_si128(x_ptr + i);
        __m128i y_value = _mm_loadu_si128(y_ptr + i);

        // Create underflow mask: an underflow occurs if b > a
        // Note: There is no unsigned comparison, so we XOR the
        // arguments with 0x80000000 to simulate a signed comparison
        //
        // We can simulate a unsigned comparison with a signed one by
        // considering this truth table:
        //
        // A > B --> C
        //
        // sign(A)  sign(B)  C (signed)  C (unsigned)
        // ---------------------------------------------
        // +        +         A > B       A > B
        // +        -         true        false
        // -        +         false       true
        // -        -         A > B       A > B
        //
        // source:
        // https://software.intel.com/en-us/forums/intel-c-compiler/topic/310260
        //
        // In our case if the sign bit of the z values are set but not
        // in the corresponding y values we have an underflow.
        __m128i underflow = _mm_cmpgt_epi32(_mm_xor_si128(mask, y_value),
                                            _mm_xor_si128(mask, x_value));
        // Subtract the second value
        x_value = _mm_sub_epi32(x_value, y_value);
        // Subtract 5 if an underflow occurred
        x_value = _mm_sub_epi32(x_value, _mm_and_si128(underflow, five));
        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr + i, x_value);
    }

    return simd_size * 16;
}

std::size_t prime2325_sse42::vector_multiply_into(uint8_t* x, uint32_t constant,
                                                  std::size_t size) const
{
    assert(x != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16;

    __m128i lowmask = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);
    __m128i five = _mm_set1_epi32(5);
    __m128i prime64 =
        _mm_set_epi32(0, prime2325().prime(), 0, prime2325().prime());

    // Store 4 copies of the 32-bit constant factor
    __m128i b = _mm_set1_epi32(constant);

    __m128i* x_ptr = (__m128i*)x;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 16-bytes of the destination buffer
        __m128i a = _mm_loadu_si128(x_ptr + i);

        // Each 128-bit register contains four 32-bit unsigned integers
        // Multiply integer pairs 0 and 2 (we obtain two 64-bit products)
        __m128i c1 = _mm_mul_epu32(a, b);
        // Multiply integer pairs 1 and 3 (we obtain two 64-bit products)
        __m128i c2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));

        // Operate on the first pair of 64-bit products
        multiply_helper(c1, lowmask, five, prime64);

        // Operate on the second pair of 64-bit products
        multiply_helper(c2, lowmask, five, prime64);

        // Shuffle the 4 products to the correct position as defined in
        // this post: http://stackoverflow.com/a/10501533
        __m128i c =
            _mm_unpacklo_epi32(_mm_shuffle_epi32(c1, _MM_SHUFFLE(0, 0, 2, 0)),
                               _mm_shuffle_epi32(c2, _MM_SHUFFLE(0, 0, 2, 0)));

        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr + i, c);
    }

    return simd_size * 16;
}

std::size_t prime2325_sse42::vector_multiply_add_into(uint8_t* x,
                                                      const uint8_t* y,
                                                      uint32_t constant,
                                                      std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16U;

    __m128i mask = _mm_set1_epi32(0x80000000);
    __m128i lowmask = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);
    __m128i five = _mm_set1_epi32(5);
    __m128i prime = _mm_set1_epi32(prime2325().prime());
    __m128i prime64 =
        _mm_set_epi32(0, prime2325().prime(), 0, prime2325().prime());
    // XOR-ed prime that will be used in signed comparisons
    __m128i xprime = _mm_set1_epi32(prime2325().prime() ^ 0x80000000);

    // Store 4 copies of the 32-bit constant factor
    __m128i b = _mm_set1_epi32(constant);

    __m128i* x_ptr = (__m128i*)x;
    const __m128i* y_ptr = (const __m128i*)y;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Multiply the src with the constant

        // Load the next 16-bytes of the source buffer
        __m128i a = _mm_loadu_si128(y_ptr + i);

        // Each 128-bit register contains four 32-bit unsigned integers
        // Multiply integer pairs 0 and 2 (we obtain two 64-bit products)
        __m128i c1 = _mm_mul_epu32(a, b);
        // Multiply integer pairs 1 and 3 (we obtain two 64-bit products)
        __m128i c2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));

        // Operate on the first pair of 64-bit products
        multiply_helper(c1, lowmask, five, prime64);

        // Operate on the second pair of 64-bit products
        multiply_helper(c2, lowmask, five, prime64);

        // Shuffle the 4 products to the correct position as defined in
        // this post: http://stackoverflow.com/a/10501533
        __m128i c =
            _mm_unpacklo_epi32(_mm_shuffle_epi32(c1, _MM_SHUFFLE(0, 0, 2, 0)),
                               _mm_shuffle_epi32(c2, _MM_SHUFFLE(0, 0, 2, 0)));

        // Add this product to the dest

        __m128i d = _mm_loadu_si128(x_ptr + i);

        add_helper(d, c, mask, five, prime, xprime);

        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr + i, d);
    }

    return simd_size * 16;
}

std::size_t prime2325_sse42::vector_multiply_subtract_into(
    uint8_t* x, const uint8_t* y, uint32_t constant, std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16;

    __m128i mask = _mm_set1_epi32(0x80000000);
    __m128i lowmask = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);
    __m128i five = _mm_set1_epi32(5);
    __m128i prime64 =
        _mm_set_epi32(0, prime2325().prime(), 0, prime2325().prime());

    // Store 4 copies of the 32-bit constant factor
    __m128i b = _mm_set1_epi32(constant);

    __m128i* x_ptr = (__m128i*)x;
    const __m128i* y_ptr = (const __m128i*)y;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Multiply the src with the constant

        // Load the next 16-bytes of the source buffer
        __m128i a = _mm_loadu_si128(y_ptr + i);

        // Each 128-bit register contains four 32-bit unsigned integers
        // Multiply integer pairs 0 and 2 (we obtain two 64-bit products)
        __m128i c1 = _mm_mul_epu32(a, b);
        // Multiply integer pairs 1 and 3 (we obtain two 64-bit products)
        __m128i c2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));

        // Operate on the first pair of 64-bit products
        multiply_helper(c1, lowmask, five, prime64);

        // Operate on the second pair of 64-bit products
        multiply_helper(c2, lowmask, five, prime64);

        // Shuffle the 4 products to the correct position as defined in
        // this post: http://stackoverflow.com/a/10501533
        __m128i c =
            _mm_unpacklo_epi32(_mm_shuffle_epi32(c1, _MM_SHUFFLE(0, 0, 2, 0)),
                               _mm_shuffle_epi32(c2, _MM_SHUFFLE(0, 0, 2, 0)));

        // Subtract this product from the dest

        __m128i d = _mm_loadu_si128(x_ptr + i);

        // Create underflow mask: an underflow occurs if c > d
        // Note: There is no unsigned comparison, so we XOR the
        // arguments with 0x80000000 to simulate a signed comparison
        __m128i underflow =
            _mm_cmpgt_epi32(_mm_xor_si128(mask, c), _mm_xor_si128(mask, d));

        // Subtract the second value
        d = _mm_sub_epi32(d, c);
        // Subtract 5 if an underflow occurred
        d = _mm_sub_epi32(d, _mm_and_si128(underflow, five));

        // Store the result in the destination buffer
        _mm_storeu_si128(x_ptr + i, d);
    }

    return simd_size * 16;
}

bool prime2325_sse42::is_compiled()
{
    return true;
}

#else

std::size_t prime2325_sse42::vector_add_into(uint8_t*, const uint8_t*,
                                             std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t prime2325_sse42::vector_subtract_into(uint8_t*, const uint8_t*,
                                                  std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t prime2325_sse42::vector_multiply_add_into(uint8_t*, const uint8_t*,
                                                      uint32_t,
                                                      std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t prime2325_sse42::vector_multiply_into(uint8_t*, uint32_t,
                                                  std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t prime2325_sse42::vector_multiply_subtract_into(uint8_t*,
                                                           const uint8_t*,
                                                           uint32_t,
                                                           std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool prime2325_sse42::is_compiled()
{
    return false;
}

#endif

}
}
}
