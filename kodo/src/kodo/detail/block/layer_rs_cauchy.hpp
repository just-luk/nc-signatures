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
#include "../rs_cauchy_matrix.hpp"
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
struct layer_rs_cauchy : public Super
{
public:
    using field_type = typename Super::field_type;

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
        m_next_repair_symbol = symbol_index{0};
        assert(m_symbols < symbol_count{field_type::max_value});
        m_bytes = Super::elements_to_bytes(m_symbols);
    }

    /// Return the number of symbols in the block
    auto symbols() const -> symbol_count
    {
        return m_symbols;
    }

    /// Return the number of repair symbols available for the block
    auto repair_symbols() const -> symbol_count
    {
        return symbol_count{field_type::max_value} - m_symbols;
    }

    /// Return the number of repair symbols remaining
    auto remaining_repair_symbols() const -> symbol_count
    {
        assert(m_next_repair_symbol <= repair_symbols());

        return repair_symbols() - symbol_count{m_next_repair_symbol.value};
    }

    // Generate next set of coefficients. A limit number of repair
    // coefficients can be generated.
    // This function must not be called more than repair_symbols() times.
    // This limitation has been made to maintain the MDS property of Cauchy RS.
    detail::symbol_index generate(uint8_t* data)
    {
        assert(remaining_repair_symbols() != symbol_count{0});
        generate_specific(data, m_next_repair_symbol);
        auto tmp = m_next_repair_symbol;
        ++m_next_repair_symbol;
        return tmp;
    }

    // Generate specific set of coefficients. This is usually the function to
    // use at the decoder side.
    void generate_specific(uint8_t* data, detail::symbol_index index) const
    {
        assert(index < repair_symbols());
        std::copy_n(m_matrix.row(index), m_bytes.value, data);
    }

    /// Return the maximum number of bytes which will be generated
    auto max_coefficients_bytes() const -> byte_count
    {
        return m_bytes;
    }

private:
    /// The number of bytes to be generated
    byte_count m_bytes;

    /// The number of symbols
    symbol_count m_symbols;

    /// The index of the next coefficients
    symbol_index m_next_repair_symbol;

    symbol_count m_remaining_repair_symbols;

    const rs_cauchy_matrix<field_type> m_matrix;
};
}
}
}
}
