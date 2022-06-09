// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
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
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include "rs_cauchy_matrix.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
using namespace fifi::field;
// Default backend implementations
static constexpr rs_cauchy_matrix_backend<binary4> binary4_backend{};
static constexpr rs_cauchy_matrix_backend<binary8> binary8_backend{};

template <>
rs_cauchy_matrix<binary4>::rs_cauchy_matrix() : m_backend(&binary4_backend)
{
}

template <>
rs_cauchy_matrix<binary8>::rs_cauchy_matrix() : m_backend(&binary8_backend)
{
}
}
}
}
