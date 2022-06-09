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

#include "binary_neon.hpp"

#include <cassert>

#include <platform/config.hpp>

// Include ARM NEON intrinsics
#if defined(PLATFORM_NEON)
#include <arm_neon.h>
#endif

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
constexpr char binary_neon::name[];

finite_field binary_neon::field() const
{
    return fifi::finite_field::binary;
}

binary binary_neon::field_info() const
{
    return binary();
}

#ifdef PLATFORM_NEON

std::size_t binary_neon::vector_add_into(uint8_t* x, const uint8_t* y,
                                         std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    // We loop 16 bytes at-a-time so we calculate how many loops we need
    std::size_t simd_size = size / 16;

    std::size_t optimized = simd_size / 8;
    std::size_t unoptimized = simd_size - (optimized * 8);

    const auto xor_helper = [&]()
    {
        // Load the next 16-bytes of the destination buffers
        uint8x16_t x_reg = vld1q_u8(x);
        uint8x16_t y_reg = vld1q_u8(y);

        // XOR these values together
        uint8x16_t result = veorq_u8(x_reg, y_reg);
        // Store the result in the destination buffer
        vst1q_u8(x, result);

        x += 16;
        y += 16;
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

bool binary_neon::is_compiled()
{
    return true;
}

#else

std::size_t binary_neon::vector_add_into(uint8_t*, const uint8_t*,
                                         std::size_t) const
{
    assert(0 && "Target platform or compiler does not support this "
                "implementation");

    return 0;
}

bool binary_neon::is_compiled()
{
    return false;
}

#endif
}
}
}
