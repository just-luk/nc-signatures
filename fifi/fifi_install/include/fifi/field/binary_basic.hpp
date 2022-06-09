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

#include "binary.hpp"

#include "../finite_field.hpp"
#include "../simd.hpp"
#include "../version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
/// Finite field computations in the binary field. This implementation is
/// platform / CPU architecture independent.
class binary_basic
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::no_simd;

    /// The name of this acceleration
    static constexpr char name[] = "binary_basic";

public:
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

    /// @copydoc acceleration::vector_add_into()
    std::size_t vector_add_into(uint8_t* x, const uint8_t* y,
                                std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_into()
    std::size_t vector_multiply_into(uint8_t* x, uint32_t constant,
                                     std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_add_into()
    std::size_t vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                         uint32_t constant,
                                         std::size_t size) const;

    /// @copydoc acceleration::vector_dot_product()
    std::size_t vector_dot_product(uint8_t** x, const uint8_t** y,
                                   const uint8_t** constants, std::size_t size,
                                   std::size_t x_vectors,
                                   std::size_t y_vectors) const;

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary field_info() const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();
};

}
}
}
