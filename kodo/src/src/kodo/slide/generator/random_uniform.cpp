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

#include "random_uniform.hpp"

#include "../../version.hpp"

#include "../../detail/slide/stack_random_uniform.hpp"

#include "../../detail/layer_final.hpp"
#include "../../detail/layer_log.hpp"
#include "../../detail/layer_mapping.hpp"
#include "../../detail/layer_math.hpp"
#include "../../detail/layer_random_uniform.hpp"
#include "../../detail/slide/layer_coefficients_view_math.hpp"
#include "../../detail/slide/layer_log_generator.hpp"
#include "../../detail/slide/layer_mapping.hpp"
#include "../../detail/slide/layer_uniform_generator.hpp"
#include "../../detail/symbol_count.hpp"
#include "../../detail/symbol_index.hpp"

#include <cassert>

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
namespace generator
{

/// Implementation interface
struct random_uniform::interface
{
    virtual ~interface(){};
    virtual void generate(uint8_t* data, const detail::symbol_range& range) = 0;
    virtual auto field() const -> finite_field = 0;
    virtual void set_seed(uint64_t seed) = 0;
    virtual auto coefficients_bytes(const detail::symbol_range& range) const
        -> detail::byte_count = 0;
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
struct model : random_uniform::interface
{
    using stack_type = kodo::detail::slide::stack_random_uniform<Math>;

    void generate(uint8_t* data, const detail::symbol_range& range) override
    {
        m_stack.generate(data, range);
    }
    auto field() const -> finite_field override
    {
        return m_stack.field();
    }

    void set_seed(uint64_t seed) override
    {
        m_stack.set_seed(seed);
    }

    auto coefficients_bytes(const detail::symbol_range& range) const
        -> detail::byte_count override
    {
        return m_stack.coefficients_bytes(range);
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

static std::unique_ptr<random_uniform::interface> make_stack(finite_field field)
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
random_uniform::random_uniform()
{
}

random_uniform::random_uniform(finite_field field) : m_impl(make_stack(field))
{
}

random_uniform::random_uniform(random_uniform&& other) :
    m_impl(std::move(other.m_impl))
{
}

random_uniform& random_uniform::operator=(random_uniform&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

random_uniform::~random_uniform()
{
}

auto random_uniform::field() const -> finite_field
{
    assert(m_impl);
    return m_impl->field();
}

void random_uniform::generate(uint8_t* coefficients, range window)
{
    assert(m_impl);

    detail::symbol_index lower_bound{window.lower_bound()};
    detail::symbol_index upper_bound{window.upper_bound()};

    m_impl->generate(coefficients,
                     detail::symbol_range{lower_bound, upper_bound});
}

void random_uniform::set_seed(uint64_t seed)
{
    assert(m_impl);
    m_impl->set_seed(seed);
}

auto random_uniform::coefficients_bytes(range window) const -> std::size_t
{
    assert(m_impl);

    detail::symbol_index lower_bound{window.lower_bound()};
    detail::symbol_index upper_bound{window.upper_bound()};

    return m_impl
        ->coefficients_bytes(detail::symbol_range{lower_bound, upper_bound})
        .value;
}

void random_uniform::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void random_uniform::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto random_uniform::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}

void random_uniform::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto random_uniform::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}
}
}
}
}
