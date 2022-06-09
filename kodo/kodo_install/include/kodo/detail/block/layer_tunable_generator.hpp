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

#include <ranbo/xoshiro256ss.h>

#include "../byte_count.hpp"
#include "../random_sequence_index.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

namespace block
{

template <class Super>
struct layer_tunable_generator : public Super
{
public:
    /// The data type used to store random values
    using result_type = uint32_t;

    struct config : public Super::config
    {
        symbol_count symbols{0};
    };

    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_symbols = config.symbols;
        m_symbol_selection = random_sequence_index(m_symbols);

        assert(m_symbols > symbol_count{0});
        m_bytes = Super::elements_to_bytes(m_symbols);
        set_seed(0);
    }

    /// @param seed Seed the generator. Using the same seed on the encoder
    ///        and decoder side will ensure that the same id is generated.
    void set_seed(uint64_t seed)
    {
        ranbo_xoshiro256ss_set_seed(&m_generator, seed);
        m_symbol_selection.set_seed(seed);
    }

    /// Return the number of symbols in the block
    auto symbols() const -> symbol_count
    {
        return m_symbols;
    }

    void generate(uint8_t* data, float density)
    {
        generate_partial(data, m_symbols, density);
    }

    void generate_partial(uint8_t* data, symbol_count symbols, float density)
    {
        assert(symbols <= m_symbols);
        assert(symbols > symbol_count{0});
        std::size_t non_zero_symbols = std::max(
            std::size_t{1}, (std::size_t)std::ceil(symbols.value * density));

        while (non_zero_symbols != 0)
        {
            auto index = m_symbol_selection.index();
            m_symbol_selection.advance();
            if (index >= symbols)
            {
                continue;
            }
            non_zero_symbols -= 1;

            Super::set_value(data, index, non_zero_coefficient());
        }
    }

    /// Return the maximum number of bytes which will be generated
    auto max_coefficients_bytes() const -> byte_count
    {
        return m_bytes;
    }

private:
    uint64_t non_zero_coefficient()
    {
        uint64_t max_value = Super::field_info().max_value;
        // generate number between 1 and max_value
        return ranbo_xoshiro256ss_generate(&m_generator) % max_value + 1;
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;

    /// The number of bytes to be generated
    byte_count m_bytes;

    /// The number of symbols
    symbol_count m_symbols;

    random_sequence_index m_symbol_selection;
};
}
}
}
}
