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

#include "symbol_count.hpp"
#include "symbol_range.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
template <class Super>
struct layer_symbols : public Super
{
public:
    struct config : public Super::config
    {
        symbol_count symbols{0};
    };

public:
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_symbols = config.symbols;
        assert(m_symbols > symbol_count{0});
    }

    /// Return the number of symbols in the block
    auto symbols() const -> symbol_count
    {
        return m_symbols;
    }

    /// return the range of symbol indicies.
    detail::symbol_range symbol_range() const
    {
        return detail::symbol_range{symbol_index{0},
                                    symbol_index{m_symbols.value}};
    }

    /// Return the last symbol
    auto last_symbol() const -> symbol_index
    {
        return symbol_index{m_symbols.value - 1U};
    }

private:
    /// The number of symbols
    symbol_count m_symbols;
};
}
}
}
