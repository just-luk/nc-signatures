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
#include "binary8_avx2.hpp"
#include "binary8_basic.hpp"
#include "binary8_neon.hpp"
#include "binary8_ssse3.hpp"
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
/// Finite field operations in the binary extension field 2⁸.
class binary8_backend
{
public:
    /// Constructor
    constexpr binary8_backend(binary8 field) :
        m_field(field), m_binary_sse2(), m_binary_avx2(), m_binary_neon(),
        m_binary_basic(), m_binary8_basic(field), m_binary8_ssse3(field),
        m_binary8_avx2(field), m_binary8_neon(field)
    {
    }

public:
    /// The field used to initialize the backend
    binary8 m_field;

    /// SSE2 acceleration for binary operations
    binary_sse2 m_binary_sse2;

    /// AVX2 acceleration for binary operations
    binary_avx2 m_binary_avx2;

    /// NEON acceleration for binary operations
    binary_neon m_binary_neon;

    /// Basic binary operations
    binary_basic m_binary_basic;

    /// Basic binary8 operations
    binary8_basic m_binary8_basic;

    /// SSSE3 binary8 operations
    binary8_ssse3 m_binary8_ssse3;

    /// AVX2 binary8 operations
    binary8_avx2 m_binary8_avx2;

    /// NEON acceleration for binary8 operations
    binary8_neon m_binary8_neon;
};

}
}
}
