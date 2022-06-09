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

#include "../detail/array_data.hpp"
#include "../detail/log_table.hpp"
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
/// Finite field computations in binary4 using standard lookup tables to
/// accelerate computations. This implementation is platform / CPU architecture
/// independent.
class binary4_basic
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::no_simd;

    /// The name of this acceleration
    static constexpr char name[] = "binary4_basic";

public:
    /// Construct with default lookup table
    constexpr binary4_basic(binary4 field) noexcept :
        m_field(field), m_multiply(), m_inverse()
    {
        detail::log_table<binary4> lut(m_field);

        for (uint32_t i = 0; i < 16; ++i)
        {
            for (uint32_t j = 0; j < 256; ++j)
            {
                uint32_t low_half = j & 0x0f;
                uint32_t high_half = (j & 0xf0) >> 4;

                uint32_t low_result = lut.multiply(i, low_half);
                uint32_t high_result = lut.multiply(i, high_half);

                uint32_t result = (high_result << 4) | low_result;

                m_multiply[i][j] = result;
            }
        }

        m_inverse[0] = 0xff;
        for (uint32_t i = 1; i < 16; ++i)
        {
            m_inverse[i] = lut.invert(i);
        }
    }

    /// @copydoc math::add()
    uint32_t add(uint32_t a, uint32_t b) const;

    /// @copydoc math::subtract()
    uint32_t subtract(uint32_t a, uint32_t b) const;

    /// @copydoc math::multiply()
    uint32_t multiply(uint32_t a, uint32_t b) const;

    /// @copydoc math::divide()
    uint32_t divide(uint32_t a, uint32_t b) const;

    /// @copydoc math::invert()
    uint32_t invert(uint32_t a) const;

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

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary4 field_info() const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();

private:
    /// The finite field used to generate this lookup table
    binary4 m_field;

    /// Table data for the multiplication of a 4 bit field element with
    /// all possible values of a byte.
    detail::table_data<uint8_t, 16, 256> m_multiply;

    /// Table data for the inverse of all 4 bit field elements
    detail::array_data<uint8_t, 16> m_inverse;
};

}
}
}
