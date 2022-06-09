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

#include "../version.hpp"
#include "symbol_count.hpp"
#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// @brief The symbol decoding status counter maintains
/// information about how many symbols are in which state.
///
/// On the decoder side there are three states a symbol can be in
/// information about these states can be found in the
/// symbol_decoding_status_tracker layer. This layer maintains
/// counters showing how many symbols are in each state.
template <class Super>
class layer_decoding_status_counter : public Super
{
public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_symbols_missing = config.symbols;
        m_symbols_partially_decoded = symbol_count{0};
        m_symbols_decoded = symbol_count{0};
    }

    void set_symbol_missing(symbol_index index)
    {
        assert(index <= Super::last_symbol());

        if (Super::is_symbol_partially_decoded(index))
        {
            ++m_symbols_missing;

            assert(m_symbols_missing <= Super::symbols());
            assert(m_symbols_partially_decoded > symbol_count{0});

            --m_symbols_partially_decoded;
        }
        else if (Super::is_symbol_decoded(index))
        {
            ++m_symbols_missing;

            assert(m_symbols_missing <= Super::symbols());
            assert(m_symbols_decoded > symbol_count{0});

            --m_symbols_decoded;
        }

        Super::set_symbol_missing(index);
    }

    void set_symbol_partially_decoded(symbol_index index)
    {
        assert(index <= Super::last_symbol());

        if (Super::is_symbol_missing(index))
        {
            ++m_symbols_partially_decoded;

            assert(m_symbols_partially_decoded <= Super::symbols());
            assert(m_symbols_missing > symbol_count{0});

            --m_symbols_missing;
        }
        else if (Super::is_symbol_decoded(index))
        {
            ++m_symbols_partially_decoded;

            assert(m_symbols_partially_decoded <= Super::symbols());
            assert(m_symbols_decoded > symbol_count{0});

            --m_symbols_decoded;
        }

        Super::set_symbol_partially_decoded(index);
    }

    void set_symbols_decoded()
    {
        m_symbols_decoded = Super::symbols();
        m_symbols_missing = symbol_count{0};
        m_symbols_partially_decoded = symbol_count{0};

        Super::set_symbols_decoded();
    }

    void set_symbol_decoded(symbol_index index)
    {
        assert(index <= Super::last_symbol());

        if (Super::is_symbol_missing(index))
        {
            ++m_symbols_decoded;

            assert(m_symbols_decoded <= Super::symbols());
            assert(m_symbols_missing > symbol_count{0});

            --m_symbols_missing;
        }
        else if (Super::is_symbol_partially_decoded(index))
        {
            ++m_symbols_decoded;

            assert(m_symbols_decoded <= Super::symbols());
            assert(m_symbols_partially_decoded > symbol_count{0});

            --m_symbols_partially_decoded;
        }

        Super::set_symbol_decoded(index);
    }

    symbol_count symbols_missing() const
    {
        return m_symbols_missing;
    }

    symbol_count symbols_partially_decoded() const
    {
        return m_symbols_partially_decoded;
    }

    symbol_count symbols_decoded() const
    {
        return m_symbols_decoded;
    }

    symbol_count rank() const
    {
        return symbols_partially_decoded() + symbols_decoded();
    }

public:
    /// Counter for keeping track of the currently missing symbols.
    symbol_count m_symbols_missing{0};

    /// Counter for symbols which are partially decoded.
    symbol_count m_symbols_partially_decoded{0};

    /// Counter for symbols which are fully decoded.
    symbol_count m_symbols_decoded{0};
};
}
}
}
