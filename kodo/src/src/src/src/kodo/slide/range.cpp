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

#include "range.hpp"

#include <bourne/json.hpp>

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{
range::range(uint64_t lower_bound, uint64_t upper_bound) :
    m_lower_bound(lower_bound), m_upper_bound(upper_bound)
{
    assert(m_lower_bound <= m_upper_bound);
}

auto range::symbol_count() const -> uint64_t
{
    return m_upper_bound - m_lower_bound;
}

auto range::lower_bound() const -> uint64_t
{
    return m_lower_bound;
}

auto range::upper_bound() const -> uint64_t
{
    return m_upper_bound;
}
}
}
}
