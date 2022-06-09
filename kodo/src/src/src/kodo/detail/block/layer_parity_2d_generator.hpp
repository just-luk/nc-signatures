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

#include "../byte_count.hpp"
#include "../generator_position.hpp"
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
struct layer_parity_2d_generator : public Super
{
public:
    enum class parity_type
    {
        source,
        row_repair,
        column_repair
    };

public:
    struct config : public Super::config
    {
        symbol_count rows{0};
        symbol_count columns{0};
    };

public:
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        assert(config.rows > detail::symbol_count{0});
        assert(config.columns > detail::symbol_count{0});

        m_rows = config.rows;
        m_columns = config.columns;
        m_bytes = Super::elements_to_bytes(symbols());
        m_max_position = generator_position{
            static_cast<std::size_t>(m_rows.value * m_columns.value +
                                     (m_rows.value + m_columns.value) - 1)};
    }

    /// Return the number of rows in the block
    auto rows() const -> symbol_count
    {
        return m_rows;
    }

    /// Return the number of columns in the block
    auto columns() const -> symbol_count
    {
        return m_columns;
    }

    /// Return the number of symbols in the block
    auto symbols() const -> symbol_count
    {
        return symbol_count{m_rows.value * m_columns.value};
    }

    /// Return the generators max position.
    auto max_position() const -> generator_position
    {
        return m_max_position;
    }

    /// Return the maximum number of bytes which will be generated
    auto max_coefficients_bytes() const -> byte_count
    {
        return m_bytes;
    }

    /// Converts a position into a type and index.
    /// The type specifies whether the given position should result in either
    /// row repair, column repair or just a plain source symbol.'
    /// @param position the position of the generator.
    /// @return the type and index for the given position.
    auto parity_type_at(generator_position position) const
        -> std::tuple<parity_type, std::size_t>
    {
        assert(position <= max_position());

        // Example
        // s denotes source
        // r denotes row repair
        // c denotes column repair
        // The subscripted numbers denotes the position
        //
        // matrix 3 x 4
        //     s₀  s₁  s₂  s₃  r₄
        //     s₅  s₆  s₇  s₈  r₉
        //     s₁₀ s₁₂ s₁₄ s₁₆ r₁₈
        //     c₁₁ c₁₃ c₁₅ c₁₇

        // Catching r₁₈
        if (max_position() == position)
        {
            return std::make_pair(parity_type::row_repair, m_rows.value - 1);
        }

        auto last_row = (m_columns.value + 1) * (m_rows.value - 1);
        if (position.value < last_row)
        {
            // Catching r₄ and r₉
            if ((position.value + 1) % (m_columns.value + 1) == 0)
            {
                return std::make_pair(parity_type::row_repair,
                                      position.value / (m_columns.value + 1));
            }
        }
        else
        {
            // Catching c₁₁ c₁₃ c₁₅ c₁₇
            auto pos = position.value - last_row;
            if (pos % 2 != 0)
            {
                return std::make_pair(parity_type::column_repair, pos / 2);
            }
        }

        // Catching sₓ
        return std::make_pair(parity_type::source, 0);
    }

    // Returns true if a coefficient vector can be generated for the given
    // index.
    auto can_generate_at(generator_position position) const -> bool
    {
        assert(position <= max_position());
        std::size_t index;
        parity_type type;
        std::tie(type, index) = parity_type_at(position);
        return type != parity_type::source;
    }

    /// Generate the coefficients for a specific position.
    /// @param coefficients The data pointer containing the coefficients.
    /// @param position The specific position to generate coefficients for.
    void generate_specific(uint8_t* data, generator_position position) const
    {
        assert(data != nullptr);
        assert(position <= max_position());

        // Zero buffer
        Super::zero_data(data, m_bytes);

        std::size_t index;
        parity_type type;
        std::tie(type, index) = parity_type_at(position);
        switch (type)
        {
        case parity_type::row_repair:
            generate_row(index, data);
            break;
        case parity_type::column_repair:
            generate_column(index, data);
            break;
        case parity_type::source:
        default:
            assert(false && "Unable to generate coeffiecnts.");
            break;
        }
    }

private:
    void generate_row(std::size_t row, uint8_t* coefficients) const
    {
        assert(row < m_rows.value);
        auto first_element = row * m_columns.value;
        for (size_t i = 0; i < m_columns.value; i++)
        {
            auto element = first_element + i;
            Super::set_value(coefficients, symbol_index{element}, 1);
        }
    }
    void generate_column(std::size_t column, uint8_t* coefficients) const
    {
        assert(column < m_columns.value);
        auto first_element = column;
        for (size_t i = 0; i < m_rows.value; i++)
        {
            auto element = first_element + i * m_columns.value;
            Super::set_value(coefficients, symbol_index{element}, 1);
        }
    }

private:
    /// The number of bytes to be generated
    byte_count m_bytes{0};

    /// The number of rows
    symbol_count m_rows{0};

    /// The number of columns
    symbol_count m_columns{0};

    /// The max position
    generator_position m_max_position{0};
};
}
}
}
}
