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

#include "decoder.hpp"

#include "../detail/fulcrum/stack_decoder.hpp"

#include "../version.hpp"

#include <cassert>

#include <fifi/field/binary16_math.hpp>
#include <fifi/field/binary4_math.hpp>
#include <fifi/field/binary8_math.hpp>
#include <fifi/field/binary_math.hpp>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace fulcrum
{
// Implementation interface

struct decoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::symbol_count symbols,
                           detail::byte_count symbol_bytes,
                           detail::symbol_count expansion) = 0;
    virtual void reset() = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto inner_field() const -> finite_field = 0;
    virtual auto symbol_bytes() const -> detail::byte_count = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual auto expansion() const -> detail::symbol_count = 0;
    virtual auto inner_symbols() const -> detail::symbol_count = 0;
    virtual auto block_bytes() const -> detail::byte_count = 0;
    virtual auto symbol_data(detail::symbol_index index) const -> uint8_t* = 0;
    virtual void set_symbol_storage(uint8_t* symbol_storage,
                                    detail::symbol_index index) = 0;
    virtual void set_symbols_storage(uint8_t* symbols_storage) = 0;
    virtual void decode_symbol(uint8_t* symbol, uint8_t* coefficients) = 0;
    virtual void decode_systematic_symbol(const uint8_t* symbol,
                                          detail::symbol_index index) = 0;
    virtual void recode_symbol(uint8_t* symbol_data, uint8_t* coefficients,
                               const uint8_t* coefficients_in) const = 0;
    virtual auto rank() const -> detail::symbol_count = 0;
    virtual auto inner_rank() const -> detail::symbol_count = 0;
    virtual auto is_complete() const -> bool = 0;
    virtual auto is_symbol_pivot(detail::symbol_index index) const -> bool = 0;
    virtual auto is_inner_symbol_pivot(detail::symbol_index index) const
        -> bool = 0;
    virtual auto is_symbol_decoded(detail::symbol_index index) const
        -> bool = 0;
    virtual void on_symbol_decoded(const symbol_decoded_callback& callback,
                                   void* user_data) = 0;
    virtual void enable_log(const log_callback& callback, void* user_data) = 0;
    virtual void disable_log() = 0;
    virtual auto is_log_enabled() const -> bool = 0;
    virtual void set_log_name(const std::string& name) = 0;
    virtual auto log_name() const -> std::string = 0;
};

namespace
{
// Implementation stack
template <class Math>
struct model : decoder::interface
{
    using stack_type = kodo::detail::fulcrum::stack_decoder<Math>;

    void configure(detail::symbol_count symbols,
                   detail::byte_count symbol_bytes,
                   detail::symbol_count expansion) override
    {
        typename stack_type::config config;
        config.symbols = symbols;
        config.symbol_bytes = symbol_bytes;
        config.expansion = expansion;
        m_stack.configure(config);
    }

    void reset() override
    {
        m_stack.reset();
    }

    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    auto inner_field() const -> finite_field override
    {
        return m_stack.inner_field();
    }

    auto symbol_bytes() const -> detail::byte_count override
    {
        return m_stack.symbol_bytes();
    }

    auto symbols() const -> detail::symbol_count override
    {
        return m_stack.symbols();
    }

    auto expansion() const -> detail::symbol_count override
    {
        return m_stack.expansion();
    }

    auto inner_symbols() const -> detail::symbol_count override
    {
        return m_stack.inner_symbols();
    }

    auto block_bytes() const -> detail::byte_count override
    {
        return m_stack.block_bytes();
    }

    auto symbol_data(detail::symbol_index index) const -> uint8_t* override
    {
        return m_stack.symbol_data(index);
    }

    void set_symbol_storage(uint8_t* symbol_storage,
                            detail::symbol_index index) override
    {
        m_stack.set_symbol_storage(symbol_storage, index);
    }

    void set_symbols_storage(uint8_t* symbols_storage) override
    {
        m_stack.set_symbols_storage(symbols_storage);
    }

    void decode_symbol(uint8_t* symbol, uint8_t* coefficients) override
    {
        m_stack.decode_symbol(symbol, coefficients);
    }

    void decode_systematic_symbol(const uint8_t* symbol,
                                  detail::symbol_index index) override
    {
        m_stack.decode_systematic_symbol(symbol, index);
    }

    void recode_symbol(uint8_t* symbol, uint8_t* coefficients,
                       const uint8_t* coefficients_in) const override
    {
        m_stack.recode_symbol(symbol, coefficients, coefficients_in);
    }

    auto rank() const -> detail::symbol_count override
    {
        return m_stack.rank();
    }

    auto inner_rank() const -> detail::symbol_count override
    {
        return m_stack.inner_rank();
    }

    auto is_complete() const -> bool override
    {
        return m_stack.is_complete();
    }

    auto is_inner_symbol_pivot(detail::symbol_index index) const
        -> bool override
    {
        return m_stack.is_inner_symbol_pivot(index);
    }

    auto is_symbol_pivot(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_symbol_pivot(index);
    }

    auto is_symbol_decoded(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_symbol_decoded(index);
    }

    void on_symbol_decoded(const symbol_decoded_callback& callback,
                           void* user_data) override
    {
        m_stack.on_symbol_decoded(callback, user_data);
    }

    void enable_log(const log_callback& callback, void* user_data) override
    {
        m_stack.enable_log(callback, user_data);
    }

    void disable_log() override
    {
        m_stack.disable_log();
    }

    auto is_log_enabled() const -> bool override
    {
        return m_stack.is_log_enabled();
    }

    void set_log_name(const std::string& name) override
    {
        m_stack.set_log_name(name);
    }

    auto log_name() const -> std::string override
    {
        return m_stack.log_name();
    }

private:
    stack_type m_stack;
};

static std::unique_ptr<decoder::interface> make_stack(finite_field field)
{
    switch (field)
    {
    case finite_field::binary:
        return std::make_unique<model<fifi::field::binary_math>>();
    case finite_field::binary4:
        return std::make_unique<model<fifi::field::binary4_math>>();
    case finite_field::binary8:
        return std::make_unique<model<fifi::field::binary8_math>>();
    case finite_field::binary16:
        return std::make_unique<model<fifi::field::binary16_math>>();
    default:
        assert(0 && "Not a valid field");
        return nullptr;
    }
}
}

decoder::decoder()
{
}

decoder::decoder(finite_field field) : m_impl(make_stack(field))
{
}

decoder::decoder(decoder&& other) : m_impl(std::move(other.m_impl))
{
}

decoder& decoder::operator=(decoder&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

decoder::~decoder()
{
}

void decoder::configure(std::size_t symbols, std::size_t symbol_bytes,
                        std::size_t expansion)
{
    assert(m_impl);
    return m_impl->configure(detail::symbol_count{symbols},
                             detail::byte_count{symbol_bytes},
                             detail::symbol_count{expansion});
}

void decoder::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

auto decoder::field() const -> finite_field
{
    assert(m_impl);
    return m_impl->field();
}

auto decoder::inner_field() const -> finite_field
{
    assert(m_impl);
    return m_impl->inner_field();
}

auto decoder::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto decoder::expansion() const -> uint64_t
{
    assert(m_impl);
    return m_impl->expansion().value;
}

auto decoder::inner_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->inner_symbols().value;
}

auto decoder::symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes().value;
}

auto decoder::block_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->block_bytes().value;
}

void decoder::set_symbols_storage(uint8_t* symbols_storage)
{
    assert(m_impl);
    m_impl->set_symbols_storage(symbols_storage);
}

void decoder::set_symbol_storage(uint8_t* symbol_storage, uint64_t index)
{
    assert(m_impl);
    m_impl->set_symbol_storage(symbol_storage, detail::symbol_index{index});
}

auto decoder::symbol_data(uint64_t index) const -> const uint8_t*
{
    assert(m_impl);
    return m_impl->symbol_data(detail::symbol_index{index});
}

void decoder::decode_symbol(uint8_t* symbol, uint8_t* coefficients)
{
    assert(m_impl);
    m_impl->decode_symbol(symbol, coefficients);
}

void decoder::decode_systematic_symbol(const uint8_t* symbol, uint64_t index)
{
    assert(m_impl);
    m_impl->decode_systematic_symbol(symbol, detail::symbol_index{index});
}

void decoder::recode_symbol(uint8_t* symbol, uint8_t* coefficients,
                            const uint8_t* coefficients_in) const
{
    assert(m_impl);
    m_impl->recode_symbol(symbol, coefficients, coefficients_in);
}

auto decoder::rank() const -> uint64_t
{
    assert(m_impl);
    return m_impl->rank().value;
}

auto decoder::inner_rank() const -> uint64_t
{
    assert(m_impl);
    return m_impl->inner_rank().value;
}

auto decoder::is_complete() const -> bool
{
    assert(m_impl);
    return m_impl->is_complete();
}

auto decoder::is_symbol_pivot(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->is_symbol_pivot(detail::symbol_index{index});
}

auto decoder::is_inner_symbol_pivot(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->is_inner_symbol_pivot(detail::symbol_index{index});
}

auto decoder::is_symbol_decoded(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->is_symbol_decoded(detail::symbol_index{index});
}

void decoder::on_symbol_decoded(const symbol_decoded_callback& callback,
                                void* user_data)
{
    assert(m_impl);
    m_impl->on_symbol_decoded(callback, user_data);
}

void decoder::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void decoder::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto decoder::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}

void decoder::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto decoder::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}
}
}
}
