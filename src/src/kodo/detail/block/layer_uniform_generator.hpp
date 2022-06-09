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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <list>

#include "../byte_count.hpp"
#include "../is_zero.hpp"
#include "../symbol_frame.hpp"
#include "../symbol_range.hpp"
#include "../to_string.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{

/// This layer generates the coding coefficients used for the encoding /
/// decoding a block code.
///
/// It requires a layer underneath that supports generating coefficients.
template <class Super>
struct layer_uniform_generator : public Super
{
    void generate(uint8_t* data)
    {
        auto bytes = Super::max_coefficients_bytes();
        do
        {
            Super::generate_data(data, bytes);
            // If we generated all-zero we retry
        } while (is_zero(data, bytes));
    }

    void generate_partial(uint8_t* data, symbol_count symbols)
    {
        assert(symbols > symbol_count{0});

        auto bytes = Super::elements_to_bytes(symbols);
        auto range = Super::to_symbol_range(symbol_index{0U}, symbols);
        auto frame = Super::to_symbol_frame(range);

        detail::symbol_range zero_right{range.upper_bound(),
                                        frame.upper_bound()};
        do
        {
            Super::generate_data(data, bytes);

            for (symbol_index i : zero_right)
            {
                Super::set_value(data, relative_index(frame, i), 0U);
            }
            // If we generated all-zero we retry
        } while (is_zero(data, bytes));
    }
};
}
}
}
}
