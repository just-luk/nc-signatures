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

#include "tunable.hpp"

#include "../../version.hpp"

#include "../../detail/block/stack_tunable.hpp"

#include "../../detail/block/layer_tunable_generator.hpp"
#include "../../detail/layer_final.hpp"
#include "../../detail/layer_log.hpp"
#include "../../detail/layer_mapping.hpp"
#include "../../detail/layer_math.hpp"
#include "../../detail/symbol_count.hpp"
#include "../../detail/symbol_index.hpp"

#include <cassert>

#include <fifi/field/prime2325_math.hpp>
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
namespace generator
{

/// Implementation interface
struct tunable::interface
{
    virtual ~interface(){};
    virtual void generate(uint8_t* data, float density) = 0;
    virtual void generate_partial(uint8_t* data, detail::symbol_count symbols,
                                  float density) = 0;
    virtual void reset() = 0;
    virtual auto field() const -> finite_field = 0;
    virtual auto symbols() const -> detail::symbol_count = 0;
    virtual void configure(detail::symbol_count symbols) = 0;
    virtual void set_seed(uint64_t seed) = 0;
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
struct model : tunable::interface
{
    using stack_type = kodo::detail::block::stack_tunable<Math>;

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

    void generate(uint8_t* data, float density) override
    {
        m_stack.generate(data, density);
    }

    void generate_partial(uint8_t* data, detail::symbol_count symbols,
                          float density) override
    {
        m_stack.generate_partial(data, symbols, density);
    }

    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    auto symbols() const -> detail::symbol_count override
    {
        return m_stack.symbols();
    }

    void set_seed(uint64_t seed) override
    {
        m_stack.set_seed(seed);
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

static std::unique_ptr<tunable::interface> make_stack(finite_field field)
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
    case finite_field::prime2325:
        return std::make_unique<model<fifi::field::prime2325_math>>();
    default:
        assert(0 && "Not a valid field");
        return nullptr;
    }
}
}

tunable::tunable()
{
}

tunable::tunable(finite_field field) : m_impl(make_stack(field))
{
}

tunable::tunable(tunable&& other) : m_impl(std::move(other.m_impl))
{
}

tunable& tunable::operator=(tunable&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

tunable::~tunable()
{
}

void tunable::configure(std::size_t symbols)
{
    assert(m_impl);
    m_impl->configure(detail::symbol_count{symbols});
}

void tunable::reset()
{
    assert(m_impl);
    m_impl->reset();
}

auto tunable::field() const -> finite_field
{
    assert(m_impl);
    return m_impl->field();
}

auto tunable::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

void tunable::generate(uint8_t* coefficients, float density)
{
    assert(m_impl);
    m_impl->generate(coefficients, density);
}

void tunable::generate_partial(uint8_t* coefficients, uint64_t symbols,
                               float density)
{
    assert(m_impl);
    m_impl->generate_partial(coefficients, detail::symbol_count{symbols},
                             density);
}

void tunable::set_seed(uint64_t seed)
{
    assert(m_impl);
    m_impl->set_seed(seed);
}

auto tunable::max_coefficients_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_coefficients_bytes().value;
}

void tunable::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void tunable::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto tunable::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}

void tunable::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto tunable::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}
}
}
}
}
