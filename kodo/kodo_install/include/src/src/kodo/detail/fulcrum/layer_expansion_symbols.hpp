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
#include <cstring>
#include <vector>

#include "../symbol_count.hpp"

#include "../../block/generator/random_uniform.hpp"
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
struct layer_expansion_symbols : public Super
{
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_expansion_symbols.resize(Super::expansion().value);
        for (auto& symbol : m_expansion_symbols)
        {
            symbol.resize(Super::symbol_bytes().value);
        }
    }

    uint8_t* expansion_symbol_data(symbol_index index)
    {
        assert(index >= Super::symbols());
        return m_expansion_symbols.at((index - Super::symbols()).value).data();
    }

    const uint8_t* expansion_symbol_data(symbol_index index) const
    {
        assert(index >= Super::symbols());
        return m_expansion_symbols.at((index - Super::symbols()).value).data();
    }

private:
    // Storage for the outer code symbols
    std::vector<std::vector<uint8_t>> m_expansion_symbols;
};
}
}
}
}
