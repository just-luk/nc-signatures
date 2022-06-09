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

#include "../byte_count.hpp"
#include "../in_range.hpp"
#include "../symbol_count.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_count.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// @brief Tracks the number of symbols in the three different states.
template <class Super>
class layer_decoding_status_counter : public Super
{
public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_symbols_missing = symbol_count{0};
        m_symbols_partially_decoded = symbol_count{0};
        m_symbols_decoded = symbol_count{0};
    }

    /// All symbols start out in the missing state
    void push_symbol()
    {
        ++m_symbols_missing;
        Super::push_symbol();
    }

    /// When popping a symbol we have to update the counters according to the
    /// symbol's state.
    auto pop_symbol() -> uint8_t*
    {
        symbol_range stream_range = Super::stream_range();

        symbol_index pop_index = stream_range.lower_bound();

        if (Super::is_symbol_missing(pop_index))
        {
            assert(m_symbols_missing > symbol_count{0U});
            --m_symbols_missing;
        }
        else if (Super::is_symbol_partially_decoded(pop_index))
        {
            assert(m_symbols_partially_decoded > symbol_count{0U});
            --m_symbols_partially_decoded;
        }
        else
        {
            assert(Super::is_symbol_decoded(pop_index));
            assert(m_symbols_decoded > symbol_count{0U});
            --m_symbols_decoded;
        }

        return Super::pop_symbol();
    }

    /// Set the symbol missing
    void set_symbol_missing(symbol_index index)
    {
        assert(detail::in_range(Super::stream_range(), index));
        assert(Super::is_symbol_partially_decoded(index));

        ++m_symbols_missing;
        assert(m_symbols_partially_decoded > symbol_count{0});
        assert(m_symbols_missing <=
               detail::to_symbol_count(Super::stream_range()));

        --m_symbols_partially_decoded;

        Super::set_symbol_missing(index);
    }

    /// Set the symbol partially decoded
    void set_symbol_partially_decoded(symbol_index index)
    {
        assert(detail::in_range(Super::stream_range(), index));
        assert(Super::is_symbol_missing(index));

        ++m_symbols_partially_decoded;
        assert(m_symbols_missing > symbol_count{0});
        assert(m_symbols_partially_decoded <=
               detail::to_symbol_count(Super::stream_range()));

        --m_symbols_missing;

        Super::set_symbol_partially_decoded(index);
    }

    /// Set the symbol fully decoded
    void set_symbol_decoded(symbol_index index)
    {
        assert(detail::in_range(Super::stream_range(), index));
        assert(!Super::is_symbol_decoded(index));

        assert(m_symbols_decoded <=
               detail::to_symbol_count(Super::stream_range()));
        ++m_symbols_decoded;

        if (Super::is_symbol_missing(index))
        {
            assert(m_symbols_missing > symbol_count{0});
            --m_symbols_missing;
        }
        else if (Super::is_symbol_partially_decoded(index))
        {
            assert(m_symbols_partially_decoded > symbol_count{0});
            --m_symbols_partially_decoded;
        }

        Super::set_symbol_decoded(index);
    }

    /// Return the number of missing symbols
    auto symbols_missing() const -> symbol_count
    {
        return m_symbols_missing;
    }

    /// Return the number of partially decoded symbols
    auto symbols_partially_decoded() const -> symbol_count
    {
        return m_symbols_partially_decoded;
    }

    /// Return the number of fully decoded symbols
    auto symbols_decoded() const -> symbol_count
    {
        return m_symbols_decoded;
    }

    /// Return the rank
    auto rank() const -> symbol_count
    {
        return symbols_decoded() + symbols_partially_decoded();
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
}
