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
#include <cstdint>

#include <ranbo/xoshiro256ss.h>

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// Random number generator outer code in the perpetual code
template <class InnerCoefficientsType>
struct outer_generator
{
    outer_generator()
    {
        set_seed(0);
    }

    /// Seed the outer generator
    void set_seed(uint64_t seed)
    {
        ranbo_xoshiro256ss_set_seed(&m_generator, seed);
    }

    /// Generate the indicies of the outer code
    void generate(InnerCoefficientsType* coefficients, std::size_t segments)
    {
        for (std::size_t i = 0; i < segments; ++i)
        {
            coefficients[i] = ranbo_xoshiro256ss_generate(&m_generator);
        }
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;
};

}
}
}
}
