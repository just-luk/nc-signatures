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

#include "binary4_neon.hpp"

#include <cassert>

#include <platform/config.hpp>

// Include ARM NEON intrinsics
#if defined(PLATFORM_NEON)
#include <arm_neon.h>
#endif

#include "get_value.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char binary4_neon::name[];

finite_field binary4_neon::field() const
{
    return fifi::finite_field::binary4;
}

binary4 binary4_neon::field_info() const
{
    return m_field;
}

#ifdef PLATFORM_NEON

std::size_t binary4_neon::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                                   uint32_t constant,
                                                   std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert(constant < 16);

    // Initialize the look-up tables
    // Load the 16-byte row that contains pre-calculated multiplication
    // results with the low-half of the constant
    uint8x16_t t1 = vld1q_u8(m_low[constant]);
    // Convert to uint8x8x2_t as vtbl2_u8 expects two 8-byte arrays
    uint8x8x2_t table1 = {{vget_low_u8(t1), vget_high_u8(t1)}};

    // table2 contains the results with the high-half of the constant
    uint8x16_t t2 = vld1q_u8(m_high[constant]);
    uint8x8x2_t table2 = {{vget_low_u8(t2), vget_high_u8(t2)}};

    // Create low and high bitmasks by replicating the mask values 16 times
    uint8x16_t mask1 = vdupq_n_u8((uint8_t)0x0f);
    uint8x16_t mask2 = vdupq_n_u8((uint8_t)0xf0);

    std::size_t simd_size = size / 16;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 16-bytes of the source buffer
        uint8x16_t y_value = vld1q_u8(y + (i * 16));

        // Apply mask1 to get the low-half of the data
        uint8x16_t l = vandq_u8(y_value, mask1);

        // Perform 8 simultaneous table lookups to multiply the low-half
        // The lookup is performed twice due to NEON restrictions
        l = vcombine_u8(vtbl2_u8(table1, vget_low_u8(l)),
                        vtbl2_u8(table1, vget_high_u8(l)));

        // Apply mask2 to get the high-half of the data
        uint8x16_t h = vandq_u8(y_value, mask2);

        // Right shift the high-half by 4 bits to get values in [0,15]
        h = vshrq_n_u8(h, 4);

        // Perform table lookup with these indices to multiply the high-half
        // The lookup is performed twice due to NEON restrictions
        h = vcombine_u8(vtbl2_u8(table2, vget_low_u8(h)),
                        vtbl2_u8(table2, vget_high_u8(h)));

        // XOR the high and low halves together to get the final result
        uint8x16_t result = veorq_u8(h, l);

        // Load the next 16-bytes of the destination buffer
        uint8x16_t x_value = vld1q_u8(x + (i * 16));

        // XOR the multiplied z value and the y value
        result = veorq_u8(x_value, result);

        // Store the result in the destination buffer
        vst1q_u8(x + (i * 16), result);
    }

    return simd_size * 16;
}

std::size_t binary4_neon::vector_multiply_into(uint8_t* x, uint32_t constant,
                                               std::size_t size) const
{
    assert(x != nullptr);
    assert(size > 0U);
    assert(constant < 16U);

    // Initialize the look-up tables
    // Load the 16-byte row that contains pre-calculated multiplication
    // results with the low-half of the constant
    uint8x16_t t1 = vld1q_u8(m_low[constant]);
    // Convert to uint8x8x2_t as vtbl2_u8 expects two 8-byte arrays
    uint8x8x2_t table1 = {{vget_low_u8(t1), vget_high_u8(t1)}};

    // table2 contains the results with the high-half of the constant
    uint8x16_t t2 = vld1q_u8(m_high[constant]);
    uint8x8x2_t table2 = {{vget_low_u8(t2), vget_high_u8(t2)}};

    // Create low and high bitmasks by replicating the mask values 16 times
    uint8x16_t mask1 = vdupq_n_u8((uint8_t)0x0f);
    uint8x16_t mask2 = vdupq_n_u8((uint8_t)0xf0);

    std::size_t simd_size = size / 16;

    for (std::size_t i = 0; i < simd_size; ++i)
    {
        // Load the next 16-bytes of the destination buffer
        uint8x16_t q0 = vld1q_u8(x + (i * 16));

        // Apply mask1 to get the low-half of the data
        uint8x16_t l = vandq_u8(q0, mask1);

        // Perform 8 simultaneous table lookups to multiply the low-half
        // The lookup is performed twice due to NEON restrictions
        l = vcombine_u8(vtbl2_u8(table1, vget_low_u8(l)),
                        vtbl2_u8(table1, vget_high_u8(l)));

        // Apply mask2 to get the high-half of the data
        uint8x16_t h = vandq_u8(q0, mask2);

        // Right shift the high-half by 4 bits to get values in [0,15]
        h = vshrq_n_u8(h, 4);

        // Perform table lookup with these indices to multiply the high-half
        // The lookup is performed twice due to NEON restrictions
        h = vcombine_u8(vtbl2_u8(table2, vget_low_u8(h)),
                        vtbl2_u8(table2, vget_high_u8(h)));

        // XOR the high and low halves together to get the final result
        uint8x16_t result = veorq_u8(h, l);

        // Store the result in the destination buffer
        vst1q_u8(x + (i * 16), result);
    }

    return simd_size * 16;
}

std::size_t binary4_neon::vector_dot_product(uint8_t** x, const uint8_t** y,
                                             const uint8_t** constants,
                                             std::size_t size,
                                             std::size_t x_vectors,
                                             std::size_t y_vectors) const
{
    assert(x != nullptr);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    std::size_t simd_size = (size / 16) * 16;

    if (simd_size == 0)
        return 0;

    for (std::size_t i = 0; i < x_vectors; ++i)
    {
        // Zero-initialize the destination buffer
        std::fill_n(x[i], size, 0);

        for (std::size_t j = 0; j < y_vectors; ++j)
        {
            // The vectors are multiplied and added into the result
            uint8_t constant = field::get_value(binary4(), constants[i], j);
            auto processed =
                vector_multiply_add_into(x[i], y[j], constant, size);
            assert(processed == simd_size);
        }
    }

    return simd_size;
}

bool binary4_neon::is_compiled()
{
    return true;
}

#else

std::size_t binary4_neon::vector_multiply_add_into(uint8_t*, const uint8_t*,
                                                   uint32_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary4_neon::vector_multiply_into(uint8_t*, uint32_t,
                                               std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

std::size_t binary4_neon::vector_dot_product(uint8_t**, const uint8_t**,
                                             const uint8_t**, std::size_t,
                                             std::size_t, std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool binary4_neon::is_compiled()
{
    return false;
}

#endif
}
}
}
