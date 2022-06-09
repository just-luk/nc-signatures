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

#include "../optional_index.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// Simple layer which keep track of wether the decoder found a pivot during the
/// last call to decode_symbol.
template <class Super>
struct layer_decoder_found_pivot : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_pivot = optional_index::not_found();
    }

    void decode_symbol(uint8_t* symbol, inner_coefficients_type coefficients,
                       symbol_index offset)
    {
        m_pivot = Super::decode_symbol(symbol, coefficients, offset);
    }

    auto pivot_found() const -> bool
    {
        return m_pivot.found();
    }

    auto pivot() const -> symbol_index
    {
        return m_pivot.index();
    }

private:
    optional_index m_pivot = optional_index::not_found();
};
}
}
}
}
