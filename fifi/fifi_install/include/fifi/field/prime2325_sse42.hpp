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

#include "../finite_field.hpp"
#include "../simd.hpp"
#include "../version.hpp"
#include "prime2325.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field computations in 2³²-5. This implementation depends on sse42
/// instructions
class prime2325_sse42
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::sse42;

    /// The name of this acceleration
    static constexpr char name[] = "prime2325_sse42";

public:
    /// @copydoc math::vector_add_into()
    std::size_t vector_add_into(uint8_t* x, const uint8_t* y,
                                std::size_t size) const;

    /// @copydoc math::vector_subtract_into()
    std::size_t vector_subtract_into(uint8_t* x, const uint8_t* y,
                                     std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_into()
    std::size_t vector_multiply_into(uint8_t* x, uint32_t constant,
                                     std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_add_into()
    std::size_t vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                         uint32_t constant,
                                         std::size_t size) const;

    /// @copydoc acceleration::vector_multiply_subtract_into()
    std::size_t vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                              uint32_t constant,
                                              std::size_t size) const;

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    prime2325 field_info() const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();
};

}
}
}
