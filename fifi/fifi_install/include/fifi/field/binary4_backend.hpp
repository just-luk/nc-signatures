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
#include "binary4_avx2.hpp"
#include "binary4_basic.hpp"
#include "binary4_neon.hpp"
#include "binary4_ssse3.hpp"
#include "binary_avx2.hpp"
#include "binary_basic.hpp"
#include "binary_neon.hpp"
#include "binary_sse2.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field operations in the binary extension field 2⁴.
class binary4_backend
{
public:
    /// Constructor
    constexpr binary4_backend(binary4 field) :
        m_field(field), m_binary4_basic(field), m_binary4_ssse3(field),
        m_binary4_avx2(field), m_binary4_neon(field), m_binary_neon(),
        m_binary_sse2(), m_binary_avx2(), m_binary_basic()
    {
    }

public:
    /// The field used to initialize the backend
    binary4 m_field;

    /// Lookup table acceleration for binary4 operations
    binary4_basic m_binary4_basic;

    /// SSSE3 acceleration for binary4 operations
    binary4_ssse3 m_binary4_ssse3;

    /// AVX2 acceleration for binary4 operations
    binary4_avx2 m_binary4_avx2;

    /// Neon acceleration for binary4 operations
    binary4_neon m_binary4_neon;

    /// Neon acceleration for binary operations
    binary_neon m_binary_neon;

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
