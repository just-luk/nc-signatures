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

#include "../../version.hpp"

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../fill_zero.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class Super>
struct layer_inner_encoder : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    /// Encode a new symbol
    void encode_symbol(uint8_t* symbol_storage,
                       inner_coefficients_type coefficients,
                       symbol_index offset) const
    {
        assert(symbol_storage != nullptr);
        assert(coefficients != 0);
        assert(offset < Super::symbols());

        // Zero out the symbol buffer
        fill_zero(symbol_storage, Super::max_symbol_bytes());

        while (coefficients != 0)
        {
            if (offset >= Super::symbol_upper_bound())
            {
                return;
            }

            if (0b1 & coefficients)
            {
                const uint8_t* symbol = Super::symbol_data(offset);
                byte_count bytes = Super::symbol_bytes(offset);

                Super::vector_add_into(symbol_storage, symbol, bytes);
            }
            coefficients = coefficients >> 1U;
            ++offset;
        }
    }
};
}
}
}
}
