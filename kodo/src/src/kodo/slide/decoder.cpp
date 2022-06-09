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

#include "../detail/slide/stack_decoder.hpp"

#include "../detail/to_symbol_count.hpp"

#include "../log_callback.hpp"

#include "../version.hpp"

#include <fifi/field/binary16_math.hpp>
#include <fifi/field/binary4_math.hpp>
#include <fifi/field/binary8_math.hpp>
#include <fifi/field/binary_math.hpp>

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{

struct decoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::byte_count max_symbol_bytes) = 0;
    virtual void reset(const reset_callback& callback, void* user_data) = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto stream_range() const -> detail::symbol_range = 0;
    virtual auto is_stream_empty() const -> bool = 0;
    virtual auto in_stream(detail::symbol_index index) const -> bool = 0;
    virtual void
    set_stream_lower_bound(detail::symbol_index stream_lower_bound) = 0;
    virtual void push_symbol() = 0;
    virtual auto pop_symbol() -> uint8_t* = 0;
    virtual auto symbol_data(detail::symbol_index index) const
        -> const uint8_t* = 0;
    virtual auto symbol_data(detail::symbol_index index) -> uint8_t* = 0;
    virtual auto symbol_bytes(detail::symbol_index index) const
        -> detail::byte_count = 0;
    virtual auto max_symbol_bytes() const -> detail::byte_count = 0;
    virtual auto decode_symbol(uint8_t* symbol_data,
                               detail::byte_count symbol_bytes,
                               detail::symbol_range window,
                               uint8_t* coefficients_data) -> uint8_t* = 0;
    virtual auto decode_systematic_symbol(uint8_t* symbol_data,
                                          detail::byte_count symbol_bytes,
                                          detail::symbol_index index)
        -> uint8_t* = 0;
    virtual auto symbols_missing() const -> detail::symbol_count = 0;
    virtual auto symbols_partially_decoded() const -> detail::symbol_count = 0;
    virtual auto symbols_decoded() const -> detail::symbol_count = 0;
    virtual auto rank() const -> detail::symbol_count = 0;
    virtual bool is_symbol_decoded(detail::symbol_index index) const = 0;
    virtual bool is_symbol_pivot(detail::symbol_index index) const = 0;

    virtual void on_symbol_decoded(const symbol_decoded_callback& callback,
                                   void* user_data) = 0;
    virtual void on_symbol_pivot(const symbol_pivot_callback& callback,
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
    using stack_type = kodo::detail::slide::stack_decoder<Math>;

    void configure(detail::byte_count max_symbol_bytes) override
    {
        typename stack_type::config config;
        config.max_symbol_bytes = max_symbol_bytes;
        return m_stack.configure(config);
    }

    void reset(const reset_callback& callback, void* user_data) override
    {
        m_stack.reset(callback, user_data);
    }

    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    auto symbol_data(detail::symbol_index index) -> uint8_t* override
    {
        return m_stack.symbol_data(index);
    }

    auto symbol_data(detail::symbol_index index) const
        -> const uint8_t* override
    {
        return m_stack.symbol_data(index);
    }

    auto symbol_bytes(detail::symbol_index index) const
        -> detail::byte_count override
    {
        return m_stack.symbol_bytes(index);
    }

    auto max_symbol_bytes() const -> detail::byte_count override
    {
        return m_stack.max_symbol_bytes();
    }

    auto decode_symbol(uint8_t* symbol, detail::byte_count symbol_bytes,
                       detail::symbol_range window, uint8_t* coefficients)
        -> uint8_t* override
    {
        return m_stack.decode_symbol(symbol, symbol_bytes, window,
                                     coefficients);
    }

    auto decode_systematic_symbol(uint8_t* symbol,
                                  detail::byte_count symbol_bytes,
                                  detail::symbol_index index)
        -> uint8_t* override
    {
        return m_stack.decode_systematic_symbol(symbol, symbol_bytes, index);
    }

    auto rank() const -> detail::symbol_count override
    {
        return m_stack.rank();
    }

    auto stream_range() const -> detail::symbol_range override
    {
        return m_stack.stream_range();
    }

    auto in_stream(detail::symbol_index index) const -> bool override
    {
        return m_stack.in_stream(index);
    }

    auto is_stream_empty() const -> bool override
    {
        return m_stack.is_stream_empty();
    }

    void
    set_stream_lower_bound(detail::symbol_index stream_lower_bound) override
    {
        m_stack.set_stream_lower_bound(stream_lower_bound);
    }

    void push_symbol() override
    {
        m_stack.push_symbol();
    }

    auto pop_symbol() -> uint8_t* override
    {
        return m_stack.pop_symbol();
    }

    auto symbols_missing() const -> detail::symbol_count override
    {
        return m_stack.symbols_missing();
    }

    auto symbols_partially_decoded() const -> detail::symbol_count override
    {
        return m_stack.symbols_partially_decoded();
    }

    auto symbols_decoded() const -> detail::symbol_count override
    {
        return m_stack.symbols_decoded();
    }

    auto is_symbol_decoded(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_symbol_decoded(index);
    }

    auto is_symbol_pivot(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_symbol_pivot(index);
    }

    void on_symbol_decoded(const symbol_decoded_callback& callback,
                           void* user_data) override
    {
        m_stack.on_symbol_decoded(callback, user_data);
    }

    void on_symbol_pivot(const symbol_pivot_callback& callback,
                         void* user_data) override
    {
        m_stack.on_symbol_pivot(callback, user_data);
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

static std::unique_ptr<decoder::interface> make_stack(kodo::finite_field field)
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

decoder::decoder(kodo::finite_field field) : m_impl(make_stack(field))
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

void decoder::configure(std::size_t max_symbol_bytes)
{
    assert(m_impl);
    m_impl->configure(detail::byte_count{max_symbol_bytes});
}

void decoder::reset(const reset_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->reset(callback, user_data);
}

finite_field decoder::field() const
{
    assert(m_impl);
    return m_impl->field();
}

uint64_t decoder::stream_symbols() const
{
    assert(m_impl);
    return detail::to_symbol_count(m_impl->stream_range()).value;
}

bool decoder::is_stream_empty() const
{
    assert(m_impl);
    return m_impl->is_stream_empty();
}

uint64_t decoder::stream_lower_bound() const
{
    assert(m_impl);
    return m_impl->stream_range().lower_bound().value;
}

uint64_t decoder::stream_upper_bound() const
{
    assert(m_impl);
    return m_impl->stream_range().upper_bound().value;
}

auto decoder::stream_range() const -> range
{
    assert(m_impl);

    detail::symbol_range stream = m_impl->stream_range();
    return range{stream.lower_bound().value, stream.upper_bound().value};
}

auto decoder::in_stream(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->in_stream(detail::symbol_index{index});
}

void decoder::set_stream_lower_bound(uint64_t stream_lower_bound)
{
    assert(m_impl);
    assert(is_stream_empty());
    return m_impl->set_stream_lower_bound(
        detail::symbol_index{stream_lower_bound});
}

void decoder::push_symbol()
{
    assert(m_impl);
    m_impl->push_symbol();
}

auto decoder::pop_symbol() -> uint8_t*
{
    assert(m_impl);
    return m_impl->pop_symbol();
}

auto decoder::decode_symbol(uint8_t* symbol, std::size_t symbol_bytes,
                            range window, uint8_t* coefficients) -> uint8_t*
{
    assert(m_impl);

    detail::symbol_index lower_bound{window.lower_bound()};
    detail::symbol_index upper_bound{window.upper_bound()};

    return m_impl->decode_symbol(symbol, detail::byte_count{symbol_bytes},
                                 detail::symbol_range{lower_bound, upper_bound},
                                 coefficients);
}

auto decoder::decode_systematic_symbol(uint8_t* symbol,
                                       std::size_t symbol_bytes, uint64_t index)
    -> uint8_t*
{
    assert(m_impl);
    return m_impl->decode_systematic_symbol(
        symbol, detail::byte_count{symbol_bytes}, detail::symbol_index{index});
}

uint64_t decoder::symbols_missing() const
{
    assert(m_impl);
    return m_impl->symbols_missing().value;
}

uint64_t decoder::symbols_partially_decoded() const
{
    assert(m_impl);
    return m_impl->symbols_partially_decoded().value;
}

uint64_t decoder::symbols_decoded() const
{
    assert(m_impl);
    return m_impl->symbols_decoded().value;
}

uint64_t decoder::rank() const
{
    assert(m_impl);
    return m_impl->rank().value;
}

const uint8_t* decoder::symbol_data(uint64_t index) const
{
    assert(m_impl);
    return m_impl->symbol_data(detail::symbol_index{index});
}

uint8_t* decoder::symbol_data(uint64_t index)
{
    assert(m_impl);
    return m_impl->symbol_data(detail::symbol_index{index});
}

auto decoder::max_symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_symbol_bytes().value;
}

auto decoder::symbol_bytes(uint64_t index) const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes(detail::symbol_index{index}).value;
}

bool decoder::is_symbol_decoded(uint64_t index) const
{
    assert(m_impl);
    return m_impl->is_symbol_decoded(detail::symbol_index{index});
}

bool decoder::is_symbol_pivot(uint64_t index) const
{
    assert(m_impl);
    return m_impl->is_symbol_pivot(detail::symbol_index{index});
}

void decoder::on_symbol_decoded(const symbol_decoded_callback& callback,
                                void* user_data)
{
    assert(m_impl);
    m_impl->on_symbol_decoded(callback, user_data);
}

void decoder::on_symbol_pivot(const symbol_pivot_callback& callback,
                              void* user_data)
{
    assert(m_impl);
    m_impl->on_symbol_pivot(callback, user_data);
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
