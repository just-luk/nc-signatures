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

#include "../is_zero.hpp"
#include "../relative_index.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

/// This layer generates the coding coefficients used for the recoding
template <class Super>
struct layer_generate_recode : public Super
{
public:
    template <class Decoder>
    void generate_recode(uint8_t* data, const Decoder& decoder)
    {
        assert(data != nullptr);
        assert(decoder.inner_rank() != 0);
        assert(symbol_count{decoder.inner_rank()} <= Super::inner_symbols());

        auto bytes = Super::max_coefficients_bytes();
        auto range = Super::inner_symbol_range();
        auto frame = Super::to_symbol_frame(range);

        detail::symbol_range zero_right{range.upper_bound(),
                                        frame.upper_bound()};
        do
        {
            Super::generate_data(data, bytes);

            for (symbol_index i : range)
            {
                if (!decoder.is_inner_symbol_pivot(i.value))
                {
                    Super::set_value(data, i, 0U);
                }
            }

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
