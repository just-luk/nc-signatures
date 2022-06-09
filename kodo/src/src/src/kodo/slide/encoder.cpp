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

#include "encoder.hpp"

#include "../detail/slide/stack_encoder.hpp"

#include "../detail/to_symbol_count.hpp"

#include "../version.hpp"

#include <cassert>
#include <memory>

#include <fifi/field/binary16_math.hpp>
#include <fifi/field/binary4_math.hpp>
#include <fifi/field/binary8_math.hpp>
#include <fifi/field/binary_math.hpp>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{

struct encoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::byte_count max_symbol_bytes) = 0;
    virtual void reset(const reset_callback& callback, void* user_data) = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto stream_range() const -> detail::symbol_range = 0;
    virtual auto is_stream_empty() const -> bool = 0;
    virtual auto stream_symbols() const -> detail::symbol_count = 0;
    virtual auto in_stream(detail::symbol_index index) const -> bool = 0;
    virtual void push_symbol(const uint8_t* symbol,
                             detail::byte_count symbol_bytes) = 0;
    virtual auto pop_symbol() -> const uint8_t* = 0;
    virtual auto symbol_data(detail::symbol_index index) const
        -> const uint8_t* = 0;
    virtual auto symbol_bytes(detail::symbol_index index) const
        -> detail::byte_count = 0;
    virtual auto max_symbol_bytes() const -> detail::byte_count = 0;
    virtual detail::byte_count
    encode_systematic_symbol(uint8_t* symbol_data,
                             detail::symbol_index index) const = 0;
    virtual detail::byte_count
    encode_symbol(uint8_t* symbol_data, detail::symbol_range window,
                  const uint8_t* coefficients) const = 0;
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
struct model : encoder::interface
{
    using stack_type = kodo::detail::slide::stack_encoder<Math>;

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

    auto in_stream(detail::symbol_index index) const -> bool override
    {
        return m_stack.in_stream(index);
    }

    void push_symbol(const uint8_t* symbol, detail::byte_count bytes) override
    {
        m_stack.push_symbol(symbol, bytes);
    }

    auto pop_symbol() -> const uint8_t* override
    {
        return m_stack.pop_symbol();
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

    detail::byte_count encode_symbol(uint8_t* symbol,
                                     detail::symbol_range window,
                                     const uint8_t* coefficients) const override
    {
        return m_stack.encode_symbol(symbol, window, coefficients);
    }

    detail::byte_count
    encode_systematic_symbol(uint8_t* symbol,
                             detail::symbol_index index) const override
    {
        return m_stack.encode_systematic_symbol(symbol, index);
    }

    auto stream_range() const -> detail::symbol_range override
    {
        return m_stack.stream_range();
    }

    auto is_stream_empty() const -> bool override
    {
        return m_stack.is_stream_empty();
    }

    auto stream_symbols() const -> detail::symbol_count override
    {
        return m_stack.stream_symbols();
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

static std::unique_ptr<encoder::interface> make_stack(kodo::finite_field field)
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

encoder::encoder()
{
}

encoder::encoder(kodo::finite_field field) : m_impl(make_stack(field))
{
}

encoder::encoder(encoder&& other) : m_impl(std::move(other.m_impl))
{
}

encoder& encoder::operator=(encoder&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

encoder::~encoder()
{
}

void encoder::configure(std::size_t max_symbol_bytes)
{
    assert(m_impl);
    m_impl->configure(detail::byte_count{max_symbol_bytes});
}

void encoder::reset(const reset_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->reset(callback, user_data);
}

auto encoder::field() const -> kodo::finite_field
{
    assert(m_impl);
    return m_impl->field();
}

auto encoder::stream_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->stream_symbols().value;
}

auto encoder::is_stream_empty() const -> bool
{
    assert(m_impl);
    return m_impl->is_stream_empty();
}

auto encoder::stream_lower_bound() const -> uint64_t
{
    assert(m_impl);
    return m_impl->stream_range().lower_bound().value;
}

auto encoder::stream_upper_bound() const -> uint64_t
{
    assert(m_impl);
    return m_impl->stream_range().upper_bound().value;
}

auto encoder::stream_range() const -> range
{
    assert(m_impl);

    detail::symbol_range stream = m_impl->stream_range();
    return range{stream.lower_bound().value, stream.upper_bound().value};
}

auto encoder::in_stream(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->in_stream(detail::symbol_index{index});
}

void encoder::push_symbol(const uint8_t* symbol, std::size_t symbol_bytes)
{
    assert(m_impl);
    m_impl->push_symbol(symbol, detail::byte_count{symbol_bytes});
}

auto encoder::pop_symbol() -> const uint8_t*
{
    assert(m_impl);
    return m_impl->pop_symbol();
}

auto encoder::symbol_data(uint64_t index) const -> const uint8_t*
{
    assert(m_impl);
    return m_impl->symbol_data(detail::symbol_index{index});
}

auto encoder::symbol_bytes(uint64_t index) const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes(detail::symbol_index{index}).value;
}

auto encoder::max_symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_symbol_bytes().value;
}

std::size_t encoder::encode_systematic_symbol(uint8_t* symbol_data,
                                              uint64_t index) const
{
    assert(m_impl);
    return m_impl
        ->encode_systematic_symbol(symbol_data, detail::symbol_index{index})
        .value;
}

std::size_t encoder::encode_symbol(uint8_t* symbol_data, range window,
                                   const uint8_t* coefficients) const
{
    assert(m_impl);

    detail::symbol_index lower_bound{window.lower_bound()};
    detail::symbol_index upper_bound{window.upper_bound()};

    return m_impl
        ->encode_symbol(symbol_data,
                        detail::symbol_range{lower_bound, upper_bound},
                        coefficients)
        .value;
}

void encoder::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void encoder::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto encoder::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}
void encoder::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto encoder::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}
}
}
}
