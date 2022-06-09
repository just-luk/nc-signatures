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
/// A binary extension field with 2¹⁶ elements
struct binary16
{
    /// @copydoc field::prime_type
    using prime_type = uint32_t;

    /// @copydoc field::degree_type
    using degree_type = uint32_t;

    /// @copydoc field::order_type
    using order_type = uint32_t;

    /// @copydoc field::value_type
    using value_type = uint16_t;

    /// @copydoc field::field
    static constexpr finite_field field = finite_field::binary16;

    /// @copydoc field::order
    static constexpr order_type order = 65536U;

    /// @copydoc field::max_value
    static constexpr order_type max_value = 65535U;

    /// @copydoc field::degree
    static constexpr degree_type degree = 16U;

    /// @copydoc field::granularity
    static constexpr uint8_t granularity = sizeof(value_type);

    /// Default construct the binary16 field (default prime)
    constexpr binary16() : m_prime(0x1100B)
    {
    }

    /// Construct from a prime (in decimal representation)
    constexpr binary16(prime_type prime) : m_prime(prime)
    {
    }

    /// @copydoc field::prime()
    constexpr prime_type prime() const
    {
        return m_prime;
    }

private:
    /// The prime value for the field
    prime_type m_prime;
};

}
}
}
