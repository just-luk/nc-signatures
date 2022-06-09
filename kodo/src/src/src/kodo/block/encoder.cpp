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

#include "../detail/block/stack_encoder.hpp"
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
namespace block
{

// Implementation interface
struct encoder::interface
{
    virtual ~interface(){};
    virtual void configure(detail::symbol_count symbols,
                           detail::byte_count symbol_bytes) = 0;
    virtual void reset() = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto symbol_bytes() const -> detail::byte_count = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual auto block_bytes() const -> detail::byte_count = 0;
    virtual auto rank() const -> detail::symbol_count = 0;
    virtual void set_symbols_storage(const uint8_t* symbols_storage) = 0;
    virtual void set_symbol_storage(const uint8_t* symbol_storage,
                                    detail::symbol_index index) = 0;
    virtual void encode_symbol(uint8_t* symbol,
                               const uint8_t* coefficients) const = 0;
    virtual void encode_systematic_symbol(uint8_t* symbol,
                                          detail::symbol_index index) const = 0;
    virtual auto is_symbol_set(detail::symbol_index index) const -> bool = 0;
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
    using stack_type = kodo::detail::block::stack_encoder<Math>;

    void configure(detail::symbol_count symbols,
                   detail::byte_count symbol_bytes) override
    {
        typename stack_type::config config;
        config.symbols = symbols;
        config.symbol_bytes = symbol_bytes;
        return m_stack.configure(config);
    }

    void reset() override
    {
        m_stack.reset();
    }

    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    auto symbol_bytes() const -> detail::byte_count override
    {
        return m_stack.symbol_bytes();
    }

    auto symbols() const -> detail::symbol_count override
    {
        return m_stack.symbols();
    }

    auto block_bytes() const -> detail::byte_count override
    {
        return m_stack.block_bytes();
    }

    auto rank() const -> detail::symbol_count override
    {
        return m_stack.rank();
    }

    void set_symbol_storage(const uint8_t* symbol_storage,
                            detail::symbol_index index) override
    {
        m_stack.set_symbol_storage(symbol_storage, index);
    }

    void set_symbols_storage(const uint8_t* symbols_storage) override
    {
        m_stack.set_symbols_storage(symbols_storage);
    }

    void encode_symbol(uint8_t* symbol,
                       const uint8_t* coefficients) const override
    {
        m_stack.encode_symbol(symbol, coefficients);
    }

    void encode_systematic_symbol(uint8_t* symbol,
                                  detail::symbol_index index) const override
    {
        m_stack.encode_systematic_symbol(symbol, index);
    }

    auto is_symbol_set(detail::symbol_index index) const -> bool override
    {
        return m_stack.is_symbol_set(index);
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

static std::unique_ptr<encoder::interface> make_stack(finite_field field)
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

encoder::encoder(finite_field field) : m_impl(make_stack(field))
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

void encoder::configure(uint64_t symbols, std::size_t symbol_bytes)
{
    assert(m_impl);
    return m_impl->configure(detail::symbol_count{symbols},
                             detail::byte_count{symbol_bytes});
}

void encoder::reset()
{
    assert(m_impl);
    return m_impl->reset();
}

finite_field encoder::field() const
{
    assert(m_impl);
    return m_impl->field();
}

auto encoder::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto encoder::symbol_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->symbol_bytes().value;
}

auto encoder::block_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->block_bytes().value;
}

auto encoder::rank() const -> std::size_t
{
    assert(m_impl);
    return m_impl->rank().value;
}

void encoder::set_symbols_storage(const uint8_t* symbols_storage)
{
    assert(m_impl);
    m_impl->set_symbols_storage(symbols_storage);
}

void encoder::set_symbol_storage(const uint8_t* symbol_storage, uint64_t index)
{
    assert(m_impl);
    assert(symbol_storage != nullptr);
    assert(rank() != symbols() && "All symbols set");
    assert(!is_symbol_set(index) && "Symbol already set");

    m_impl->set_symbol_storage(symbol_storage, detail::symbol_index{index});
}

void encoder::encode_symbol(uint8_t* symbol, const uint8_t* coefficients) const
{
    assert(m_impl);
    m_impl->encode_symbol(symbol, coefficients);
}

void encoder::encode_systematic_symbol(uint8_t* symbol, uint64_t index) const
{
    assert(m_impl);
    m_impl->encode_systematic_symbol(symbol, detail::symbol_index{index});
}

auto encoder::is_symbol_set(uint64_t index) const -> bool
{
    assert(m_impl);
    assert(index < symbols());
    return m_impl->is_symbol_set(detail::symbol_index{index});
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
