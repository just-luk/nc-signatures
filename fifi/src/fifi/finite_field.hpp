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

#include "version.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
/// The finite fields
enum class finite_field : uint8_t
{
    /// The binary field
    binary,
    /// The binary extension field with 2⁴ elements
    binary4,
    /// The binary extension field with 2⁸ elements
    binary8,
    /// The binary extension field with 2¹⁶ elements
    binary16,
    /// Prime field with 2³²-5 elements
    prime2325
};
}
}
