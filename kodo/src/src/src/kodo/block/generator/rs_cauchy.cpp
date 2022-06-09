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

#include "rs_cauchy.hpp"

#include "../../version.hpp"

#include "../../detail/block/stack_rs_cauchy.hpp"

#include "../../detail/symbol_count.hpp"
#include "../../detail/symbol_index.hpp"

#include <fifi/field/binary4_math.hpp>
#include <fifi/field/binary8_math.hpp>

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace block
{
namespace generator
{

/// Implementation interface
struct rs_cauchy::interface
{
    virtual ~interface(){};
    virtual void reset() = 0;
    virtual void configure(detail::symbol_count symbols) = 0;
    virtual auto generate(uint8_t* coefficients) -> detail::symbol_index = 0;
    virtual void generate_specific(uint8_t* coefficients,
                                   detail::symbol_index index) const = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual auto repair_symbols() const -> detail::symbol_count = 0;
    virtual auto remaining_repair_symbols() const -> detail::symbol_count = 0;
    virtual auto max_coefficients_bytes() const -> detail::byte_count = 0;
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
struct model : rs_cauchy::interface
{
    using stack_type = kodo::detail::block::stack_rs_cauchy<Math>;

    void configure(detail::symbol_count symbols) override
    {
        assert(symbols > detail::symbol_count{0});

        typename stack_type::config config;
        config.symbols = symbols;
        m_stack.configure(config);
    }

    void reset() override
    {
        m_stack.reset();
    }

    auto generate(uint8_t* coefficients) -> detail::symbol_index override
    {
        return m_stack.generate(coefficients);
    }

    void generate_specific(uint8_t* coefficients,
                           detail::symbol_index position) const override
    {
        m_stack.generate_specific(coefficients, position);
    }

    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    auto symbols() const -> detail::symbol_count override
    {
        return m_stack.symbols();
    }

    auto repair_symbols() const -> detail::symbol_count override
    {
        return m_stack.repair_symbols();
    }

    auto remaining_repair_symbols() const -> detail::symbol_count override
    {
        return m_stack.remaining_repair_symbols();
    }

    auto max_coefficients_bytes() const -> detail::byte_count override
    {
        return m_stack.max_coefficients_bytes();
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

static std::unique_ptr<rs_cauchy::interface> make_stack(finite_field field)
{
    switch (field)
    {
    case finite_field::binary4:
        return std::make_unique<model<fifi::field::binary4_math>>();
    case finite_field::binary8:
        return std::make_unique<model<fifi::field::binary8_math>>();
    default:
        assert(0 && "Unsupported field");
        return nullptr;
    }
}
}

rs_cauchy::rs_cauchy()
{
}

rs_cauchy::rs_cauchy(finite_field field) : m_impl(make_stack(field))
{
}

rs_cauchy::rs_cauchy(rs_cauchy&& other) : m_impl(std::move(other.m_impl))
{
}

rs_cauchy& rs_cauchy::operator=(rs_cauchy&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

rs_cauchy::~rs_cauchy()
{
}

void rs_cauchy::configure(std::size_t symbols)
{
    assert(m_impl);
    m_impl->configure(detail::symbol_count{symbols});
}

void rs_cauchy::reset()
{
    assert(m_impl);
    m_impl->reset();
}

auto rs_cauchy::field() const -> finite_field
{
    assert(m_impl);
    return m_impl->field();
}

auto rs_cauchy::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto rs_cauchy::repair_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->repair_symbols().value;
}

auto rs_cauchy::remaining_repair_symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->remaining_repair_symbols().value;
}

auto rs_cauchy::generate(uint8_t* coefficients) -> std::size_t
{
    assert(m_impl);
    return m_impl->generate(coefficients).value;
}

void rs_cauchy::generate_specific(uint8_t* coefficients, uint64_t index) const
{
    assert(m_impl);
    m_impl->generate_specific(coefficients, detail::symbol_index{index});
}

auto rs_cauchy::max_coefficients_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_coefficients_bytes().value;
}

void rs_cauchy::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void rs_cauchy::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto rs_cauchy::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}

void rs_cauchy::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto rs_cauchy::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}
}
}
}
}
