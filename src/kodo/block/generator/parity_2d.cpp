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

#include "parity_2d.hpp"

#include "../../detail/block/stack_parity_2d.hpp"

#include "../../version.hpp"

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
struct parity_2d::interface : kodo::detail::block::stack_parity_2d
{
};

parity_2d::parity_2d() : m_impl(std::make_unique<parity_2d::interface>())
{
}

parity_2d::parity_2d(parity_2d&& other) : m_impl(std::move(other.m_impl))
{
}

parity_2d& parity_2d::operator=(parity_2d&& other)
{
    m_impl = std::move(other.m_impl);
    return *this;
}

parity_2d::~parity_2d()
{
}

void parity_2d::configure(uint64_t rows, uint64_t columns)
{
    assert(m_impl);
    parity_2d::interface::config config;
    config.rows = detail::symbol_count{rows};
    config.columns = detail::symbol_count{columns};
    m_impl->configure(config);
}

void parity_2d::reset()
{
    assert(m_impl);
    m_impl->reset();
}

auto parity_2d::rows() const -> uint64_t
{
    assert(m_impl);
    return m_impl->rows().value;
}

auto parity_2d::columns() const -> uint64_t
{
    assert(m_impl);
    return m_impl->columns().value;
}

auto parity_2d::symbols() const -> uint64_t
{
    assert(m_impl);
    return m_impl->symbols().value;
}

auto parity_2d::can_advance() const -> bool
{
    assert(m_impl);
    return m_impl->can_advance();
}

void parity_2d::advance()
{
    assert(m_impl);
    assert(can_advance());
    m_impl->advance();
}

auto parity_2d::can_generate() const -> bool
{
    assert(m_impl);
    return m_impl->can_generate();
}

auto parity_2d::generate(uint8_t* coefficients) const -> std::size_t
{
    assert(m_impl);
    assert(can_generate());
    return m_impl->generate(coefficients).value;
}

void parity_2d::generate_specific(uint8_t* coefficients,
                                  std::size_t position) const
{
    assert(m_impl);
    m_impl->generate_specific(coefficients,
                              detail::generator_position{position});
}

auto parity_2d::max_coefficients_bytes() const -> std::size_t
{
    assert(m_impl);
    return m_impl->max_coefficients_bytes().value;
}

void parity_2d::set_column_redundancy_enabled(bool enabled)
{
    assert(m_impl);
    m_impl->set_column_redundancy_enabled(enabled);
}

auto parity_2d::column_redundancy_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->column_redundancy_enabled();
}

void parity_2d::set_row_redundancy_enabled(bool enabled)
{
    assert(m_impl);
    m_impl->set_row_redundancy_enabled(enabled);
}

auto parity_2d::row_redundancy_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->row_redundancy_enabled();
}

void parity_2d::enable_log(const log_callback& callback, void* user_data)
{
    assert(m_impl);
    m_impl->enable_log(callback, user_data);
}

void parity_2d::disable_log()
{
    assert(m_impl);
    m_impl->disable_log();
}

auto parity_2d::is_log_enabled() const -> bool
{
    assert(m_impl);
    return m_impl->is_log_enabled();
}

void parity_2d::set_log_name(const std::string& name)
{
    assert(m_impl);
    m_impl->set_log_name(name);
}

auto parity_2d::log_name() const -> std::string
{
    assert(m_impl);
    return m_impl->log_name();
}

}
}
}
}
