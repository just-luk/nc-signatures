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
#include "binary8.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Implements NEON SIMD accelerated finite field arithmetics for binary8.
class binary8_neon
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::neon;

    /// The name of this acceleration
    static constexpr char name[] = "binary8_neon";

public:
#ifdef FIFI_PLATFORM_ARM
    /// Constructor
    constexpr binary8_neon(binary8 field) noexcept :
        m_field(field), m_low(), m_high()
    {
        detail::log_table<binary8> lookup_table(m_field);

        for (uint32_t i = 0; i < binary8::order; ++i)
        {
            for (uint32_t j = 0; j < 16; ++j)
            {
                uint8_t result_low = lookup_table.multiply(i, j);
                uint8_t result_high = lookup_table.multiply(i, j << 4);

                m_low[i][j] = result_low;
                m_high[i][j] = result_high;
            }
        }
    }
#else
    /// Constructor
    constexpr binary8_neon(binary8 field) noexcept : m_field(field)
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

    /// @return docs::is_compiled()
    static bool is_compiled();

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary8 field_info() const;

private:
    /// The finite field used to generate this lookup table
    binary8 m_field;

#ifdef FIFI_PLATFORM_ARM
    /// Table data for the low 4 bit field elements of a byte
    detail::table_data<uint8_t, 256, 16> m_low;

    /// Table data for the high 4 bit field elements of a byte
    detail::table_data<uint8_t, 256, 16> m_high;
#endif
};

}
}
}
