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

#include "../detail/perpetual/stack_decoder.hpp"

#include "../version.hpp"

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{
struct decoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::byte_count block_bytes,
                           detail::byte_count symbol_bytes,
                           detail::symbol_count outer_interval,
                           std::size_t outer_segments,
                           double mapping_threshold) = 0;
    virtual void reset() = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual auto data_symbols() const -> detail::symbol_count = 0;
    virtual auto outer_symbols() const -> detail::symbol_count = 0;
    virtual auto outer_interval() const -> detail::symbol_count = 0;
    virtual auto outer_segments() const -> std::size_t = 0;
    virtual auto mapping_threshold() const -> double = 0;
    virtual auto symbol_bytes() const -> detail::byte_count = 0;
    virtual auto width() const -> detail::symbol_count = 0;
    virtual auto block_bytes() const -> detail::byte_count = 0;
    virtual void set_symbols_storage(uint8_t* symbol_storage) = 0;
    virtual auto symbols_storage() const -> uint8_t* = 0;
    virtual void decode_symbol(uint8_t* symbol, uint64_t coefficients,
                               detail::symbol_index offset) = 0;
    virtual auto pivot_found() const -> bool = 0;
    virtual auto pivot() const -> detail::symbol_index = 0;
    virtual auto rank() const -> detail::symbol_count = 0;
    virtual auto is_complete() const -> bool = 0;
    virtual auto is_pivot(detail::symbol_index index) const -> bool = 0;
    virtual void enable_log(log_callback callback, void* user_data) = 0;
    virtual void disable_log() = 0;
    virtual auto is_log_enabled() const -> bool = 0;
    virtual void set_log_name(const std::string& name) = 0;
    virtual auto log_name() const -> std::string = 0;
};

namespace
{
template <class CoefficientsType>
struct model : decoder::interface
{
    using stack_type = kodo::detail::perpetual::stack_decoder<CoefficientsType>;

    void configure(detail::byte_count block_bytes,
                   detail::byte_count symbol_bytes,
                   detail::symbol_count outer_interval,
                   std::size_t outer_segments,
                   double mapping_threshold) override
    {
        typename stack_type::config config;
        config.outer_interval = kodo::detail::symbol_count{outer_interval};
        config.outer_segments = outer_segments;
        config.symbol_bytes = detail::byte_count{symbol_bytes};
        config.block_bytes = detail::byte_count{block_bytes};
        config.mapping_threshold = mapping_threshold;
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

    auto mapping_threshold() const -> double override
    {
        return m_stack.mapping_threshold();
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

    void set_symbols_storage(uint8_t* symbol_storage) override
    {
        m_stack.set_symbols_storage(symbol_storage);
    }

    auto symbols_storage() const -> uint8_t* override
    {
        return m_stack.symbols_storage();
    }

    auto rank() const -> detail::symbol_count override
    {
        return m_stack.rank();
    }

    auto is_complete() const -> bool override
    {
        return m_stack.is_complete();
    }

    auto is_pivot(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_pivot(index);
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

    void decode_symbol(uint8_t* symbol, uint64_t coefficients,
                       detail::symbol_index offset) override
    {
        m_stack.decode_symbol(symbol, coefficients, offset);
    }

    auto pivot_found() const -> bool override
    {
        return m_stack.pivot_found();
    }

    auto pivot() const -> detail::symbol_index override
    {
        return m_stack.pivot();
    }

private:
    stack_type m_stack;
};

static std::unique_ptr<decoder::interface> make_stack(perpetual::width width)
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

decoder::decoder()
{
}

decoder::decoder(perpetual::width width) : m_impl(make_stack(width))
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

void decoder::configure(std::size_t block_bytes, std::size_t symbol_bytes,
                        uint64_t outer_interval, std::size_t outer_segments,
                        double mapping_threshold)
{
    assert(m_impl);
    return m_impl->configure(detail::byte_count{block_bytes},
                             detail::byte_count{symbol_bytes},
                             detail::symbol_count{outer_interval},
                             outer_segments, mapping_threshold);
}

void decoder::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

auto decoder::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto decoder::data_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->data_symbols().value;
}

auto decoder::outer_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->outer_symbols().value;
}

auto decoder::outer_interval() const -> uint64_t
{
    assert(m_impl);
    return m_impl->outer_interval().value;
}

auto decoder::outer_segments() const -> std::size_t
{
    assert(m_impl);
    return m_impl->outer_segments();
}

auto decoder::mapping_threshold() const -> double
{
    assert(m_impl);
    return m_impl->mapping_threshold();
}

auto decoder::symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes().value;
}

auto decoder::width() const -> uint64_t
{
    assert(m_impl);
    return m_impl->width().value;
}

auto decoder::block_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->block_bytes().value;
}

void decoder::set_symbols_storage(uint8_t* symbol_storage)
{
    assert(m_impl);
    m_impl->set_symbols_storage(symbol_storage);
}

auto decoder::symbols_storage() const -> uint8_t*
{
    assert(m_impl);
    return m_impl->symbols_storage();
}

void decoder::decode_symbol(uint8_t* symbol, uint64_t coefficients,
                            uint64_t offset)
{
    assert(m_impl);
    m_impl->decode_symbol(symbol, coefficients, detail::symbol_index{offset});
}

auto decoder::pivot_found() const -> bool
{
    assert(m_impl);
    return m_impl->pivot_found();
}

auto decoder::pivot() const -> uint64_t
{
    assert(m_impl);
    return m_impl->pivot().value;
}

auto decoder::rank() const -> uint64_t
{
    assert(m_impl);
    return m_impl->rank().value;
}

auto decoder::is_complete() const -> bool
{
    assert(m_impl);
    return m_impl->is_complete();
}

auto decoder::is_pivot(uint64_t index) const -> bool
{
    assert(m_impl);
    return m_impl->is_pivot(detail::symbol_index{index});
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
