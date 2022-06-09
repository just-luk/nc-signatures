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

#include "../detail/perpetual/stack_encoder.hpp"
#include "../version.hpp"

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{

// Implementation interface
struct encoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::byte_count block_bytes,
                           detail::byte_count symbol_bytes,
                           detail::symbol_count outer_interval,
                           std::size_t outer_segments) = 0;
    virtual void reset() = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual auto data_symbols() const -> detail::symbol_count = 0;
    virtual auto outer_symbols() const -> detail::symbol_count = 0;
    virtual auto outer_interval() const -> detail::symbol_count = 0;
    virtual auto outer_segments() const -> std::size_t = 0;
    virtual auto symbol_bytes() const -> detail::byte_count = 0;
    virtual auto width() const -> detail::symbol_count = 0;
    virtual auto block_bytes() const -> detail::byte_count = 0;
    virtual void set_symbols_storage(const uint8_t* symbol_storage) = 0;
    virtual auto symbols_storage() const -> const uint8_t* = 0;
    virtual void encode_symbol(uint8_t* symbol_storage, uint64_t coefficients,
                               detail::symbol_index offset) const = 0;
    virtual void enable_log(log_callback callback, void* user_data) = 0;
    virtual void disable_log() = 0;
    virtual auto is_log_enabled() const -> bool = 0;
    virtual void set_log_name(const std::string& name) = 0;
    virtual auto log_name() const -> std::string = 0;
};

namespace
{
template <class CoefficientsType>
struct model : encoder::interface
{
    using stack_type = kodo::detail::perpetual::stack_encoder<CoefficientsType>;

    void configure(detail::byte_count block_bytes,
                   detail::byte_count symbol_bytes,
                   detail::symbol_count outer_interval,
                   std::size_t outer_segments) override
    {
        typename stack_type::config config;
        config.symbol_bytes = symbol_bytes;
        config.block_bytes = block_bytes;
        config.outer_interval = outer_interval;
        config.outer_segments = outer_segments;
        m_stack.configure(config);
    }

    void reset() override
    {
        m_stack.reset();
    }

    auto symbols() const -> detail::symbol_count override
    {
        return m_stack.symbols();
    }

    auto data_symbols() const -> detail::symbol_count override
    {
        return m_stack.data_symbols();
    }

    auto outer_symbols() const -> detail::symbol_count override
    {
        return m_stack.outer_symbols();
    }

    auto outer_interval() const -> detail::symbol_count override
    {
        return m_stack.outer_interval();
    }

    auto outer_segments() const -> std::size_t override
    {
        return m_stack.outer_segments();
    }

    auto symbol_bytes() const -> detail::byte_count override
    {
        return m_stack.max_symbol_bytes();
    }

    auto width() const -> detail::symbol_count override
    {
        return m_stack.width();
    }

    auto block_bytes() const -> detail::byte_count override
    {
        return m_stack.block_bytes();
    }

    void set_symbols_storage(const uint8_t* symbol_storage) override
    {
        m_stack.set_symbols_storage(symbol_storage);
    }

    auto symbols_storage() const -> const uint8_t* override
    {
        return m_stack.symbols_storage();
    }

    void encode_symbol(uint8_t* symbol_storage, uint64_t coefficients,
                       detail::symbol_index offset) const override
    {
        m_stack.encode_symbol(symbol_storage, coefficients, offset);
    }

    void enable_log(log_callback callback, void* user_data) override
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

static std::unique_ptr<encoder::interface> make_stack(perpetual::width width)
{
    switch (width)
    {
    case perpetual::width::_8:
        return std::make_unique<model<uint8_t>>();
    case kodo::perpetual::width::_16:
        return std::make_unique<model<uint16_t>>();
    case kodo::perpetual::width::_32:
        return std::make_unique<model<uint32_t>>();
    case kodo::perpetual::width::_64:
        return std::make_unique<model<uint64_t>>();
    default:
        assert(0 && "Not a valid field");
        return nullptr;
    }
}
}

encoder::encoder()
{
}

encoder::encoder(perpetual::width width) : m_impl(make_stack(width))
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

void encoder::configure(std::size_t block_bytes, std::size_t symbol_bytes,
                        uint64_t outer_interval, std::size_t outer_segments)
{
    assert(m_impl);
    return m_impl->configure(
        detail::byte_count{block_bytes}, detail::byte_count{symbol_bytes},
        detail::symbol_count{outer_interval}, outer_segments);
}

void encoder::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

auto encoder::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto encoder::data_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->data_symbols().value;
}

auto encoder::outer_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->outer_symbols().value;
}

auto encoder::outer_interval() const -> uint64_t
{
    assert(m_impl);
    return m_impl->outer_interval().value;
}

auto encoder::outer_segments() const -> std::size_t
{
    assert(m_impl);
    return m_impl->outer_segments();
}

auto encoder::symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes().value;
}

auto encoder::width() const -> uint64_t
{
    assert(m_impl);
    return m_impl->width().value;
}

auto encoder::block_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->block_bytes().value;
}

void encoder::set_symbols_storage(const uint8_t* symbol_storage)
{
    assert(m_impl);
    m_impl->set_symbols_storage(symbol_storage);
}

auto encoder::symbols_storage() const -> const uint8_t*
{
    assert(m_impl);
    return m_impl->symbols_storage();
}

void encoder::encode_symbol(uint8_t* symbol_storage, uint64_t coefficients,
                            uint64_t offset) const
{
    assert(m_impl);
    m_impl->encode_symbol(symbol_storage, coefficients,
                          detail::symbol_index{offset});
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
