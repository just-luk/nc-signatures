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

#pragma once

#include <cstdint>

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{
/// Range for specifying window and stream
struct range
{
    /// Create a new range
    range(uint64_t lower_bound, uint64_t upper_bound);

    /// @return The number of symbols in the range
    auto symbol_count() const -> uint64_t;

    /// @return The lower bound
    auto lower_bound() const -> uint64_t;

    /// @return The upper bound
    auto upper_bound() const -> uint64_t;

private:
    /// The lower bound
    uint64_t m_lower_bound;

    /// The upper bound
    uint64_t m_upper_bound;
};
}
}
}
