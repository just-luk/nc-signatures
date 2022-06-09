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

#include "../field_metric.hpp"
#include "../version.hpp"
#include <abacus/metric.hpp>
#include <abacus/metrics.hpp>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{

struct field_metrics
{
    abacus::metric m_no_simd;
    abacus::metric m_sse2;
    abacus::metric m_ssse3;
    abacus::metric m_sse42;
    abacus::metric m_avx2;
    abacus::metric m_neon;
    abacus::metric m_sse2_flag;
    abacus::metric m_ssse3_flag;
    abacus::metric m_sse42_flag;
    abacus::metric m_avx2_flag;
    abacus::metric m_neon_flag;
    abacus::metric m_field_degree;
    abacus::metric m_field_polynomial;
};

}
}
}