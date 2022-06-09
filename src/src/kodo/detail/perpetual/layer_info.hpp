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

#include "../../version.hpp"

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "outer_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class InnerCoefficientsType, class Super>
struct layer_info : public Super
{
public:
    /// The type used for storing each set of inner coefficients
    using inner_coefficients_type = InnerCoefficientsType;

    static auto width() -> symbol_count
    {
        return symbol_count{
            std::numeric_limits<inner_coefficients_type>::digits};
    }

    static auto zero_symbols() -> symbol_count
    {
        return width() - symbol_count{1U};
    }

    /// Return the number of bytes allocated for each symbol
    static auto coefficients_bytes() -> byte_count
    {
        return byte_count{sizeof(inner_coefficients_type)};
    }

public:
    /// The configuration object
    struct config : public Super::config
    {
        // The interval between outer code symbols. As an example an interval
        // of 8 means that there are 8 data symbols between each outer code
        // symbol
        symbol_count outer_interval{8};

        // The number of outer code segments - the outer code coding vector
        // is composed of N number of *width* bit segments. This also means that
        // the number of mixed symbols in an outer code symbol is outer_segments
        // * width.
        // The default width for this perpetual code is 32.
        std::size_t outer_segments{8};
    };

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {

        Super::configure(config);

        byte_count symbol_bytes = config.symbol_bytes;
        byte_count block_bytes = config.block_bytes;

        assert(symbol_bytes > byte_count{0});
        assert(block_bytes > byte_count{0});

        m_outer_interval = config.outer_interval;
        m_outer_segments = config.outer_segments;

        m_data_symbols =
            symbol_count(ceil_division(block_bytes.value, symbol_bytes.value));

        assert(m_data_symbols > symbol_count{0U});

        if (m_outer_interval != symbol_count{0})
        {
            m_outer_symbols = symbol_count{
                ceil_division(m_data_symbols.value, m_outer_interval.value)};
            assert(m_outer_symbols != symbol_count{0});
        }
        else
        {
            m_outer_symbols = symbol_count{0};
        }

        m_symbols = zero_symbols() + m_data_symbols + m_outer_symbols;

        assert(m_data_symbols > symbol_count{0});
    }

    auto data_symbols() const -> symbol_count
    {
        assert(m_data_symbols > symbol_count{0});

        return m_data_symbols;
    }

    auto outer_symbols() const -> symbol_count
    {
        return m_outer_symbols;
    }

    auto outer_interval() const -> symbol_count
    {

        return m_outer_interval;
    }

    auto outer_segments() const -> std::size_t
    {
        return m_outer_segments;
    }

    /// return the range of symbol indicies.
    detail::symbol_range symbol_range() const
    {
        return detail::symbol_range{symbol_index{0},
                                    symbol_index{m_symbols.value}};
    }

    /// Return the range of symbol indicies covered by an outer code symbol
    /// starting at the specified offset
    auto outer_range(symbol_index offset) const -> detail::symbol_range
    {
        assert(offset < m_symbols);

        // The number of symbols in the encoding vector of the outer code
        symbol_count outer_width{m_outer_segments * width().value};

        // We +1 on the offset since we don't want to include the outer
        // symbol in itself
        symbol_index lower_bound = offset + symbol_count{1};
        symbol_index upper_bound =
            std::min(lower_bound + outer_width, symbol_index{m_symbols.value});

        return to_symbol_range(lower_bound, upper_bound);
    }

    auto has_outer_code() const -> bool
    {
        return m_outer_interval > symbol_count{0};
    }

    /// Return the total number of symbols
    auto symbols() const -> symbol_count
    {
        assert(m_symbols > symbol_count{0});

        return m_symbols;
    }

    auto symbol_upper_bound() const -> symbol_index
    {
        return symbol_index{m_symbols.value};
    }

    /// Return the last symbol in the perpetual code
    auto last_symbol() const -> symbol_index
    {
        assert(m_symbols > symbol_count{0});

        return symbol_index{m_symbols.value - 1U};
    }

    /// Return the last outer symbol in the perpetual code
    auto last_outer_symbol() const -> outer_index
    {
        assert(m_outer_symbols > symbol_count{0});

        return outer_index{m_outer_symbols.value - 1U};
    }

    /// Return the number of bytes allocated for each symbol
    auto outer_coefficients_bytes() const -> byte_count
    {
        return byte_count{m_outer_segments * coefficients_bytes().value};
    }

private:
    /// The total number of z symbols
    symbol_count m_symbols{0};

    /// The total number of symbols
    symbol_count m_data_symbols{0};

    /// The number of symbols in the outer code
    symbol_count m_outer_symbols{0};

    /// The number of segments used for the outer code coding vector
    std::size_t m_outer_segments{0};

    /// The interval of the outer code symbols
    symbol_count m_outer_interval{0};
};
}
}
}
}
