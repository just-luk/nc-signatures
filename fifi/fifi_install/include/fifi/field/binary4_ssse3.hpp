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

#include "../detail/log_table.hpp"
#include "../detail/platform.hpp"
#include "../detail/table_data.hpp"
#include "../finite_field.hpp"
#include "../simd.hpp"
#include "../version.hpp"
#include "binary4.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Implements SSSE3 SIMD-accelerated finite field arithmetics for binary4.
/// The following intrinsics are used (which are available in the
/// following SIMD instruction sets):
///
/// _mm_load_si128 (SSE2)
/// _mm_set1_epi8 (SSE2)
/// _mm_and_si128 (SSE2)
/// _mm_shuffle_epi8 (SSSE3)
/// _mm_srli_epi64 (SSE2)
/// _mm_xor_si128 (SSE2)
/// _mm_store_si128 (SSE2)
///
/// The minimum required instruction set for this optimization is
/// the Supplemental Streaming SIMD Extension 3 (SSSE3).
class binary4_ssse3
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::ssse3;

    /// The name of this acceleration
    static constexpr char name[] = "binary4_ssse3";

public:
#ifdef FIFI_PLATFORM_X86
    /// Constructor
    constexpr binary4_ssse3(binary4 field) noexcept :
        m_field(field), m_low(), m_high()
    {
        detail::log_table<binary4> lookup_table(m_field);

        for (uint32_t i = 0; i < binary4::order; ++i)
        {
            for (uint32_t j = 0; j < binary4::order; ++j)
            {
                uint8_t result = lookup_table.multiply(i, j);

                m_low[i][j] = result;
                m_high[i][j] = result << 4;
            }
        }
    }
#else
    /// Constructor
    constexpr binary4_ssse3(binary4 field) noexcept : m_field(field)
    {
    }
#endif

    /// @copydoc acceleration::vector_multiply_add_into()
    std::size_t vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                         uint32_t constant,
                                         std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_into()
    std::size_t vector_multiply_into(uint8_t* x, uint32_t constant,
                                     std::size_t size) const;

    /// @copydoc acceleration::vector_dot_product()
    std::size_t vector_dot_product(uint8_t** x, const uint8_t** y,
                                   const uint8_t** constants, std::size_t size,
                                   std::size_t x_vectors,
                                   std::size_t y_vectors) const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary4 field_info() const;

private:
    /// The finite field used to generate this lookup table
    binary4 m_field;

#ifdef FIFI_PLATFORM_X86
    /// Table data for the low 4 bit field elements of a byte
    detail::table_data<uint8_t, 16, 16> m_low;

    /// Table data for the high 4 bit field elements of a byte
    detail::table_data<uint8_t, 16, 16> m_high;
#endif
};

}
}
}
