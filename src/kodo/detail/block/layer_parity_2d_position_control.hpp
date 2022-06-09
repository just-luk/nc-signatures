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

#include "../../version.hpp"

#include "../generator_position.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{

template <class Super>
struct layer_parity_2d_position_control : public Super
{
public:
    using parity_type = typename Super::parity_type;

public:
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_current_position = generator_position{0};
    }

    void advance()
    {
        assert(can_advance());
        ++m_current_position;
        if (!can_advance())
        {
            return;
        }
        std::size_t index;
        parity_type type;
        std::tie(type, index) = Super::parity_type_at(m_current_position);
        if (type == parity_type::row_repair && !m_row_redundancy_enabled)
        {
            advance();
        }
        if (type == parity_type::column_repair && !m_column_redundancy_enabled)
        {
            advance();
        }
    }

    /// @return true if the current position of the generator is no more than
    ///         the maximum position, otherwise false.
    auto can_advance() const -> bool
    {
        return m_current_position <= Super::max_position();
    }

    /// @return true if the generator can advance and can generate at
    ///         the current position, otherwise false
    auto can_generate() const -> bool
    {
        return can_advance() && Super::can_generate_at(m_current_position);
    }

    /// Generate the coefficients for the current position of the generator.
    /// @param coefficients The data pointer containing the coefficients.
    /// @return The current position of the generator.
    detail::generator_position generate(uint8_t* coefficients) const
    {
        Super::generate_specific(coefficients, m_current_position);
        return m_current_position;
    }

    /// Sets the column redundancy generation of generator to the specified mode
    /// @param enabled true enables column redundancy, false disables.
    void set_column_redundancy_enabled(bool enabled)
    {
        m_column_redundancy_enabled = enabled;
    }

    /// @return true if column redundancy is enabled, false otherwise.
    auto column_redundancy_enabled() const -> bool
    {
        return m_column_redundancy_enabled;
    }

    /// Sets the row redundancy generation of generator to the specified mode
    /// @param enabled true enables row redundancy, false disables.
    void set_row_redundancy_enabled(bool enabled)
    {
        m_row_redundancy_enabled = enabled;
    }

    /// @return true if row redundancy is enabled, false otherwise.
    auto row_redundancy_enabled() const -> bool
    {
        return m_row_redundancy_enabled;
    }

private:
    /// The current position
    generator_position m_current_position{0};

    /// Column redundancy is enabled by default
    bool m_column_redundancy_enabled = true;

    /// Row redundancy is enabled by default
    bool m_row_redundancy_enabled = true;
};
}
}
}
}
