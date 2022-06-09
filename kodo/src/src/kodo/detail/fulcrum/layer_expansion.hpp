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

#include "../symbol_count.hpp"
#include "../to_symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

template <class Super>
struct layer_expansion : public Super
{
    // The configuration object
    struct config : public Super::config
    {
        symbol_count expansion{0};
    };

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_expansion = config.expansion;
    }

    auto expansion() const -> detail::symbol_count
    {
        return m_expansion;
    }

    detail::symbol_range expansion_range() const
    {
        return detail::to_symbol_range(symbol_index{Super::symbols().value},
                                       m_expansion);
    }

    auto inner_symbols() const -> detail::symbol_count
    {
        return Super::symbols() + expansion();
    }

    detail::symbol_range inner_symbol_range() const
    {
        return detail::to_symbol_range(symbol_index{0}, inner_symbols());
    }

private:
    symbol_count m_expansion{0};
};
}
}
}
}
