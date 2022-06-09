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

#include "../version.hpp"
#include "prime2325.hpp"

#include "prime2325_basic.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field operations in 2³²-5.
class prime2325_backend
{
public:
    /// Constructor
    constexpr prime2325_backend() : m_field(), m_prime2325_basic()
    {
    }

public:
    /// The field used to initialize the backend
    prime2325 m_field;

    /// Basic 2³²-5 operations
    prime2325_basic m_prime2325_basic;
};

}
}
}
