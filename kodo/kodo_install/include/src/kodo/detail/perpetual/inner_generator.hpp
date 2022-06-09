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

#include <cassert>

#include <ranbo/xoshiro256ss.h>

#include "../layer_final.hpp"
#include "../layer_log.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// Random number generator for the disjoint perpetual code
template <class InnerCoefficientsType>
struct inner_generator : layer_log<layer_final>
{
    using inner_coefficients_type = InnerCoefficientsType;

    static auto width() -> symbol_count
    {
        return symbol_count{
            std::numeric_limits<inner_coefficients_type>::digits};
    }

    auto generate(uint64_t seed) -> inner_coefficients_type
    {
        ranbo_xoshiro256ss_set_seed(&m_generator, seed);

        // We want to make sure the pivot candidate is always is included, this
        // is done by always setting bit zero to 1.
        return ranbo_xoshiro256ss_generate(&m_generator) | 0b1;
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;
};

}
}
}
}
