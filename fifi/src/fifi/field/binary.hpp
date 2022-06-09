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

#include <cstdint>

#include "../finite_field.hpp"
#include "../version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
/// The binary field, containing the two elements {0,1}.
struct binary
{
    /// @copydoc field::prime_type
    using prime_type = uint32_t;

    /// @copydoc field::degree_type
    using degree_type = uint32_t;

    /// @copydoc field::order_type
    using order_type = uint32_t;

    /// @copydoc field::value_type
    using value_type = uint8_t;

    /// @copydoc field::field
    static constexpr finite_field field = finite_field::binary;

    /// @copydoc field::order
    static constexpr order_type order = 2U;

    /// @copydoc field::max_value
    static constexpr order_type max_value = 1U;

    /// @copydoc field::degree
    static constexpr degree_type degree = 1U;

    /// @copydoc field::granularity
    static constexpr uint8_t granularity = sizeof(value_type);

    /// @copydoc field::prime()
    constexpr prime_type prime() const
    {
        return 0x2;
    }
};

}
}
}
