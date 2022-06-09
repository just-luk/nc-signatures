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
#include "binary16_basic.hpp"
#include "binary_avx2.hpp"
#include "binary_basic.hpp"
#include "binary_sse2.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field operations in the binary extension field 2¹⁶.
class binary16_backend
{
public:
    /// Constructor
    constexpr binary16_backend(binary16 field) :
        m_field(field), m_binary16_basic(field), m_binary_sse2(),
        m_binary_avx2(), m_binary_basic()
    {
    }

public:
    /// The field used to initialize the backend
    binary16 m_field;

    /// Lookup table acceleration for binary16 operations
    binary16_basic m_binary16_basic;

    /// SSE2 acceleration for binary operations
    binary_sse2 m_binary_sse2;

    /// AVX2 acceleration for binary operations
    binary_avx2 m_binary_avx2;

    /// Basic binary operations
    binary_basic m_binary_basic;
};

}
}
}
