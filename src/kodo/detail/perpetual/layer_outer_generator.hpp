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

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../reverse.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "get_bit.hpp"
#include "outer_generator.hpp"
#include "symbol_type.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// The outer generator which generates coefficients for the outer code.
template <class Super>
struct layer_outer_generator : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    void generate_outer_coefficients(symbol_index index,
                                     inner_coefficients_type* coefficients)
    {
        m_generator.set_seed(index.value);
        m_generator.generate(coefficients, Super::outer_segments());
    }

private:
    /// The outer generator
    outer_generator<inner_coefficients_type> m_generator;
};

}
}
}
}
