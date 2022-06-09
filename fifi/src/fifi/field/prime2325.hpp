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
/// Practical implementations of the 2³²-5 prime field may use the
/// mapping algorithm which was proposed by Crowley et al. This
/// algorithm allows an efficient mapping of arbitrary binary data to
/// the 2³²-5 prime field.
struct prime2325
{
    /// @copydoc field::prime_type
    using prime_type = uint32_t;

    /// @copydoc field::degree_type
    using degree_type = uint32_t;

    /// @copydoc field::order_type
    using order_type = uint32_t;

    /// @copydoc field::value_type
    using value_type = uint32_t;

    /// @copydoc field::field
    static constexpr finite_field field = finite_field::prime2325;

    /// @copydoc field::order
    static constexpr order_type order = 4294967291U;

    /// @copydoc field::max_value
    static constexpr order_type max_value = 4294967290U;

    /// @copydoc field::degree
    static constexpr degree_type degree = 0U;

    /// @copydoc field::granularity
    static constexpr uint32_t granularity = sizeof(value_type);

    /// When use with the Crowley mapping algorithm
    /// the maximum size in bytes a block can be
    /// (2^29 - 1) * 4 to get bytes
    static constexpr uint32_t max_block_size = 2147483644U;

    /// When use with the Crowley mapping algorithm the maximum
    /// size in bytes a block can be 2^29 - 1 data words (where a
    /// word is 32 bits).
    static constexpr uint32_t max_block_length = 536870911U;

    /// @copydoc field::prime_type
    constexpr prime_type prime() const
    {
        return 4294967291U;
    }
};

}
}
}
