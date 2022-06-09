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
#include <vector>

#include "../bitset.hpp"
#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "symbol_type.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class Super>
struct layer_inner_coefficients : public Super
{

    using inner_coefficients_type = typename Super::inner_coefficients_type;

    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        // Resize the data structures to contain the required symbols
        symbol_count symbols = Super::symbols();

        // Pivot memory is also cleared when resizing
        m_pivots.resize(bit_count{static_cast<std::size_t>(symbols.value)});

        // Resize the memory for the coefficients, no need to explicitly zero
        // the memory since it will be overwritten when adding the new pivots
        m_coefficients.resize(symbols.value);

        // Zero the different counters
        m_data_rank = symbol_count{0};
        m_zero_rank = symbol_count{0};
        m_outer_rank = symbol_count{0};
    }

    /// Check if a given row has a pivot
    auto is_inner_pivot(symbol_index pivot_index) const -> bool
    {
        assert(pivot_index.value < m_pivots.bits().value);
        return m_pivots.get(
            bit_index{static_cast<std::size_t>(pivot_index.value)});
    }

    /// Return the rank of the data symbols in the inner code
    auto inner_data_rank() const -> symbol_count
    {
        return m_data_rank;
    }

    /// Return the rank of the zero-padded symbols in the inner code
    auto inner_zero_rank() const -> symbol_count
    {
        return m_zero_rank;
    }

    /// Return the rank of the outer code symbols in the inner code
    auto inner_outer_rank() const -> symbol_count
    {
        return m_outer_rank;
    }

    /// Return the total rank of the inner code
    auto inner_rank() const -> symbol_count
    {
        return symbol_count{m_pivots.count_ones().value};
    }

    /// Set a pivot in the inner code
    void set_inner_pivot(symbol_index pivot_index,
                         inner_coefficients_type coefficients)
    {
        assert(!is_inner_pivot(pivot_index));

        m_pivots.set(bit_index{static_cast<std::size_t>(pivot_index.value)});
        m_coefficients[pivot_index.value] = coefficients;

        symbol_type type = Super::map_to_type(pivot_index);

        if (type == symbol_type::data)
        {
            ++m_data_rank;
        }
        else if (type == symbol_type::zero)
        {
            ++m_zero_rank;
        }
        else
        {
            ++m_outer_rank;
        }
    }

    /// Set the coefficients for an inner code symbol
    void set_inner_coefficients(symbol_index pivot_index,
                                inner_coefficients_type coefficients)
    {
        assert(is_inner_pivot(pivot_index));

        m_coefficients[pivot_index.value] = coefficients;
    }

    /// Return the coefficients for an inner code symbol
    auto inner_coefficients_data(symbol_index pivot_index) const
        -> inner_coefficients_type
    {
        assert(is_inner_pivot(pivot_index));

        return m_coefficients[pivot_index.value];
    }

    auto inner_coefficients_storage() -> inner_coefficients_type*
    {
        return m_coefficients.data();
    }

    auto inner_coefficients_storage() const -> const inner_coefficients_type*
    {
        return m_coefficients.data();
    }

    auto inner_coefficients_storage_bytes() const -> byte_count
    {
        return byte_count{m_coefficients.size() *
                          sizeof(inner_coefficients_type)};
    }

    auto inner_pivots() -> bitset&
    {
        return m_pivots;
    }

    auto inner_pivots() const -> const bitset&
    {
        return m_pivots;
    }

private:
    /// Bitset for keeping track of pivots
    bitset m_pivots;

    /// Rank of the data symbols contained in the inner code
    symbol_count m_data_rank;

    /// Rank of the zero-padded symbols contained in the inner code
    symbol_count m_zero_rank;

    /// Rank of the outer code symbols contained in the inner code
    symbol_count m_outer_rank;

    /// Storage of the coding coefficients for each inner code symbol
    std::vector<inner_coefficients_type> m_coefficients;
};
}
}
}
}
