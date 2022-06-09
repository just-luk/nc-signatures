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

#include <ranbo/xoshiro256ss.h>

#include "../../version.hpp"

#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
/// This layer generates the offset for the perpetual code
template <class Super>
struct layer_uniform_offset : public Super
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
        set_seed(0);
    }

    /// @param seed Seed the generator. Using the same seed on the encoder
    /// and decoder side will ensure that the same id is generated.
    void set_seed(uint64_t seed)
    {
        assert(m_symbols > symbol_count{0} && "Configure first");
        ranbo_xoshiro256ss_set_seed(&m_generator, seed);
    }

    /// Generate and return the offset
    auto offset() -> symbol_index
    {
        assert(m_symbols > symbol_count{0} && "Configure first");

        // Compute the next random offset
        symbol_index random_offset{static_cast<std::size_t>(
            ranbo_xoshiro256ss_generate(&m_generator) % m_symbols.value)};
        return random_offset;
    }

    /// @return the number of symbols in the block
    auto symbols() const -> symbol_count
    {
        return m_symbols;
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;

    /// The number of symbols
    symbol_count m_symbols;
};
}
}
}
}
