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

#include "../../detail/perpetual/inner_generator.hpp"

#include <cassert>

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{
namespace generator
{

struct random_uniform::interface
{
    virtual ~interface(){};
    virtual uint64_t generate(uint64_t seed) = 0;
    virtual auto width() const -> detail::symbol_count = 0;
    virtual void enable_log(log_callback callback, void* user_data) = 0;
    virtual void disable_log() = 0;
    virtual auto is_log_enabled() const -> bool = 0;
    virtual void set_log_name(const std::string& name) = 0;
    virtual auto log_name() const -> std::string = 0;
};

namespace
{
template <class CoefficientsType>
struct model : random_uniform::interface
{
    using stack_type = detail::perpetual::inner_generator<CoefficientsType>;

    auto generate(uint64_t seed) -> uint64_t override
    {
        return m_stack.generate(seed);
    }

    auto width() const -> detail::symbol_count override
    {
        return m_stack.width();
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

static std::unique_ptr<random_uniform::interface>
make_stack(perpetual::width width)
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

random_uniform::random_uniform()
{
}

random_uniform::random_uniform(perpetual::width width) :
    m_impl(make_stack(width))
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

auto random_uniform::generate(uint64_t seed) -> uint64_t
{
    assert(m_impl);
    return m_impl->generate(seed);
}

auto random_uniform::width() const -> uint64_t
{
    assert(m_impl);
    return m_impl->width().value;
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
    return false;
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
    return "";
}
}
}
}
}
