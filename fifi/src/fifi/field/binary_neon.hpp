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
/// Finite field computations in the binary field with NEON
/// acceleration.
class binary_neon
{
public:
    /// The SIMD instruction set required
    static constexpr simd simd_type = simd::neon;

    /// The name of this acceleration
    static constexpr char name[] = "binary_neon";

public:
    /// Constructor
    constexpr binary_neon() = default;

    /// @copydoc acceleration::vector_add_into()
    std::size_t vector_add_into(uint8_t* x, const uint8_t* y,
                                std::size_t size) const;

    /// @copydoc acceleration::is_compiled()
    static bool is_compiled();

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary field_info() const;
};

}
}
}
