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

#include "../fill_zero.hpp"
#include "../is_zero.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{
/// @brief Implements basic fulcrum recoder.
template <class Super>
class layer_recoding_algorithm : public Super
{
public:
    void recode_symbol(uint8_t* symbol_data, uint8_t* coefficients,
                       const uint8_t* coefficients_in) const
    {
        assert(symbol_data != 0);
        assert(coefficients != 0);
        assert(coefficients_in != 0);

        fill_zero(symbol_data, m_inner_decoder.symbol_bytes());
        fill_zero(coefficients, m_elemination_decoder.max_coefficients_bytes());

        for (symbol_index index : Super::inner_symbol_range())
        {
            auto value = m_inner_decoder.get_value(coefficients_in, index);

            if (value == 0)
            {
                continue;
            }
            if (index < m_inner_decoder.symbols())
            {
                recode_step(m_inner_decoder, index, symbol_data, coefficients);
            }
            else
            {
                auto elemination_index =
                    index - m_elemination_decoder.elemination_offset();
                recode_step(m_elemination_decoder, elemination_index,
                            symbol_data, coefficients);
            }
        }
    }

    template <class Decoder>
    void recode_step(const Decoder& decoder, symbol_index index,
                     uint8_t* symbol_data, uint8_t* coefficients) const
    {
        assert(decoder.is_symbol_set(index));
        assert(decoder.is_symbol_pivot(index));

        const uint8_t* symbol_i = decoder.symbol_data(index);
        const uint8_t* coefficients_i = decoder.coefficients_data(index);

        // Did you forget to set the data on the decoder?
        assert(symbol_i != nullptr);
        assert(coefficients_i != nullptr);

        decoder.vector_add_into(coefficients, coefficients_i,
                                decoder.max_coefficients_bytes());
        decoder.vector_add_into(symbol_data, symbol_i,
                                m_inner_decoder.symbol_bytes());
    }

protected:
    using Super::m_elemination_decoder;
    using Super::m_inner_decoder;
};
}
}
}
}
