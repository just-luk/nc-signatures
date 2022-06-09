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
#include <vector>

#include <fifi/field/binary_math.hpp>
#include <fifi/utils.hpp>

#include "stack_elemination_decoder.hpp"

#include "../block/stack_decoder.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{
/// Layer for containing the decoders involved in the fulcrum decoding process
template <class Math, class Super>
struct layer_decoders : public Super
{
public:
    using elemination_decoder_type = stack_elemination_decoder;
    using inner_decoder_type = block::stack_decoder<fifi::field::binary_math>;
    using outer_decoder_type = block::stack_decoder<Math>;

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        typename outer_decoder_type::config outer_config;
        outer_config.symbols = config.symbols;
        outer_config.symbol_bytes = config.symbol_bytes;
        m_outer_decoder.configure(outer_config);

        elemination_decoder_type::config elemination_config;
        elemination_config.symbols = Super::expansion();
        elemination_config.symbol_bytes = Super::symbol_bytes();
        elemination_config.elimination_offset = Super::symbols();
        m_elemination_decoder.configure(elemination_config);

        m_elemination_decoder_storage.resize(
            m_elemination_decoder.block_bytes().value);
        m_elemination_decoder.set_symbols_storage(
            m_elemination_decoder_storage.data());

        inner_decoder_type::config inner_config;
        inner_config.symbols = Super::symbols();
        inner_config.symbol_bytes = Super::symbol_bytes();
        m_inner_decoder.configure(inner_config);

        m_inner_decoder_storage.resize(m_inner_decoder.block_bytes().value);
        m_inner_decoder.set_symbols_storage(m_inner_decoder_storage.data());

        layer_decoders::set_log_name("");
    }

    void set_symbols_storage(uint8_t* storage)
    {
        m_outer_decoder.set_symbols_storage(storage);
    }

    void set_symbol_storage(uint8_t* storage, symbol_index index)
    {
        m_outer_decoder.set_symbol_storage(storage, index);
    }

    auto symbol_data(symbol_index index) const -> uint8_t*
    {
        return m_outer_decoder.symbol_data(index);
    }

    auto is_symbol_decoded(symbol_index index) const -> bool
    {
        return m_outer_decoder.is_symbol_decoded(index);
    }

    void on_symbol_decoded(const kodo::symbol_decoded_callback& callback,
                           void* user_data)
    {
        return m_outer_decoder.on_symbol_decoded(callback, user_data);
    }

    auto is_symbol_pivot(symbol_index index) const -> bool
    {
        return m_outer_decoder.is_symbol_pivot(index);
    }

    auto rank() const -> symbol_count
    {
        return m_outer_decoder.rank();
    }

    bool is_complete() const
    {
        return m_outer_decoder.is_complete();
    }

    void set_value(uint8_t* data, symbol_index index, uint32_t value) const
    {
        m_outer_decoder.set_value(data, index, value);
    }

    auto field() const -> finite_field
    {
        return m_outer_decoder.field();
    }

    auto inner_symbol_range() const -> detail::symbol_range
    {
        return detail::to_symbol_range(symbol_index{0}, inner_symbols());
    }

    auto inner_symbols() const -> symbol_count
    {
        return m_inner_decoder.symbols() + m_elemination_decoder.symbols();
    }

    auto inner_rank() const -> symbol_count
    {
        return m_inner_decoder.rank() + m_elemination_decoder.rank();
    }

    auto inner_field() const -> finite_field
    {
        return m_inner_decoder.field();
    }

    auto is_inner_symbol_pivot(symbol_index index) const -> bool
    {
        if (index < m_inner_decoder.symbols())
        {
            return m_inner_decoder.is_symbol_pivot(index);
        }
        else
        {
            auto elemination_index =
                index - m_elemination_decoder.elemination_offset();
            return m_elemination_decoder.is_symbol_pivot(elemination_index);
        }
    }

    void enable_log(const log_callback& callback, void* user_data)
    {
        Super::enable_log(callback, user_data);
        m_inner_decoder.enable_log(callback, user_data);
        m_outer_decoder.enable_log(callback, user_data);
        m_elemination_decoder.enable_log(callback, user_data);
    }

    void disable_log()
    {
        Super::disable_log();
        m_inner_decoder.disable_log();
        m_outer_decoder.disable_log();
        m_elemination_decoder.disable_log();
    }

    void set_log_name(const std::string& name)
    {
        Super::set_log_name(name);
        if (name.empty())
        {
            m_inner_decoder.set_log_name("inner_decoder");
            m_outer_decoder.set_log_name("outer_decoder");
            m_elemination_decoder.set_log_name("elemination_decoder");
        }
        else
        {
            m_inner_decoder.set_log_name(name + ".inner_decoder");
            m_outer_decoder.set_log_name(name + ".outer_decoder");
            m_elemination_decoder.set_log_name(name + ".elemination_decoder");
        }
    }

protected:
    outer_decoder_type m_outer_decoder;
    elemination_decoder_type m_elemination_decoder;
    inner_decoder_type m_inner_decoder;

private:
    std::vector<uint8_t> m_inner_decoder_storage;
    std::vector<uint8_t> m_elemination_decoder_storage;
};
}
}
}
}
