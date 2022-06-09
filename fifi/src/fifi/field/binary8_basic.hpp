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
#include "binary8.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field computations in binary4 using standard lookup tables to
/// accelerate computations. This implementation is platform / CPU architecture
/// independent.
class binary8_basic
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::no_simd;

    /// The name of this acceleration
    static constexpr char name[] = "binary8_basic";

public:
    /// Construct with default lookup table
    constexpr binary8_basic(binary8 field) noexcept :
        m_field(field), m_multiply(), m_inverse()
    {
        detail::log_table<binary8> lookup_table(m_field);

        for (uint32_t i = 0; i < binary8::order; ++i)
        {
            for (uint32_t j = 0; j < binary8::order; ++j)
            {
                m_multiply[i][j] = lookup_table.multiply(i, j);
            }
        }

        m_inverse[0] = 0xff;
        for (uint32_t i = 1; i < binary8::order; ++i)
        {
            m_inverse[i] = lookup_table.invert(i);
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
    binary8 field_info() const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();

private:
    /// The finite field used to generate the lookup tables
    binary8 m_field;

    /// Table data for the multiplication of all possible 8 bit field
    /// elements.
    detail::table_data<uint8_t, 256, 256> m_multiply;

    /// Table data for the inverse of all 8 bit field elements
    detail::array_data<uint8_t, 256> m_inverse;
};

}
}
}
