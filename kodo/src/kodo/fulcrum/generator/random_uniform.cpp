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

#include "../../detail/fulcrum/stack_random_uniform.hpp"

#include "../../detail/symbol_count.hpp"
#include "../../detail/symbol_index.hpp"

#include <cassert>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace fulcrum
{
namespace generator
{

struct random_uniform::interface : kodo::detail::fulcrum::stack_random_uniform
{
};

random_uniform::random_uniform() :
    m_impl(std::make_unique<random_uniform::interface>())
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

void random_uniform::configure(uint64_t symbols, uint64_t expansion)
{
    assert(m_impl);
    random_uniform::interface::config config;
    config.symbols = detail::symbol_count{symbols};
    config.expansion = detail::symbol_count{expansion};
    m_impl->configure(config);
}

void random_uniform::reset()
{
    assert(m_impl);
    m_impl->reset();
}

auto random_uniform::field() const -> finite_field
{
    assert(m_impl);
    return m_impl->field();
}

auto random_uniform::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto random_uniform::expansion() const -> uint64_t
{
    assert(m_impl);
    return m_impl->expansion().value;
}

void random_uniform::generate(uint8_t* coefficients)
{
    assert(m_impl);
    m_impl->generate(coefficients);
}

void random_uniform::generate_partial(uint8_t* coefficients,
                                      std::size_t symbols)
{
    assert(m_impl);
    m_impl->generate_partial(coefficients, detail::symbol_count{symbols});
}

void random_uniform::generate_recode(uint8_t* coefficients,
                                     const decoder& decoder)
{
    assert(m_impl);
    m_impl->generate_recode(coefficients, decoder);
}

void random_uniform::set_seed(uint64_t seed)
{
    assert(m_impl);
    m_impl->set_seed(seed);
}

auto random_uniform::max_coefficients_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_coefficients_bytes().value;
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
