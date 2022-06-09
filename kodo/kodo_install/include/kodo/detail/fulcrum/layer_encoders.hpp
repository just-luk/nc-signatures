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

#include <fifi/field/binary_math.hpp>

#include "../block/stack_encoder.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

template <class Math, class Super>
struct layer_encoders : public Super
{
public:
    using inner_encoder_type = block::stack_encoder<fifi::field::binary_math>;
    using outer_encoder_type = block::stack_encoder<Math>;

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        typename outer_encoder_type::config outer_config;
        outer_config.symbols = config.symbols;
        outer_config.symbol_bytes = config.symbol_bytes;
        m_outer_encoder.configure(outer_config);

        inner_encoder_type::config inner_config;
        inner_config.symbols = Super::inner_symbols();
        inner_config.symbol_bytes = Super::symbol_bytes();
        m_inner_encoder.configure(inner_config);

        layer_encoders::set_log_name("");
    }

    /// Set the storage of the block
    void set_symbols_storage(const uint8_t* storage)
    {
        m_outer_encoder.set_symbols_storage(storage);
    }

    /// Set the storage of a symbol
    void set_symbol_storage(const uint8_t* storage, symbol_index index)
    {
        m_outer_encoder.set_symbol_storage(storage, index);
    }

    auto is_symbol_set(symbol_index index) const -> bool
    {
        return m_outer_encoder.is_symbol_set(index);
    }

    auto rank() const -> symbol_count
    {
        return m_outer_encoder.rank();
    }

    auto inner_rank() const -> symbol_count
    {
        return m_inner_encoder.rank();
    }

    void set_value(uint8_t* data, symbol_index index, uint32_t value) const
    {
        m_outer_encoder.set_value(data, index, value);
    }

    auto field() const -> finite_field
    {
        return m_outer_encoder.field();
    }

    auto inner_field() const -> finite_field
    {
        return m_inner_encoder.field();
    }

    void enable_log(const log_callback& callback, void* user_data)
    {
        Super::enable_log(callback, user_data);
        m_inner_encoder.enable_log(callback, user_data);
        m_outer_encoder.enable_log(callback, user_data);
    }

    void disable_log()
    {
        Super::disable_log();
        m_inner_encoder.disable_log();
        m_outer_encoder.disable_log();
    }

    void set_log_name(const std::string& name)
    {
        Super::set_log_name(name);
        if (name.empty())
        {
            m_inner_encoder.set_log_name("inner_encoder");
            m_outer_encoder.set_log_name("outer_encoder");
        }
        else
        {
            m_inner_encoder.set_log_name(name + ".inner_encoder");
            m_outer_encoder.set_log_name(name + ".outer_encoder");
        }
    }

protected:
    inner_encoder_type m_inner_encoder;
    outer_encoder_type m_outer_encoder;
};
}
}
}
}
