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

#pragma once

#include <cassert>
#include <cstdint>
#include <deque>

#include "../../slide/stream.hpp"
#include "../symbol_frame.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "coefficients_vector.hpp"
#include "coefficients_view.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
namespace
{
struct optional_coefficients_vector
{
    static optional_coefficients_vector unset()
    {
        return {false, coefficients_vector{}};
    }

    optional_coefficients_vector() = default;

    optional_coefficients_vector(bool is_set, coefficients_vector vector) :
        m_is_set(is_set), m_vector(std::move(vector))
    {
    }

    auto is_set() const -> bool
    {
        return m_is_set;
    }

    auto vector() -> coefficients_vector&
    {
        assert(m_is_set);
        return m_vector;
    }

    auto vector() const -> const coefficients_vector&
    {
        assert(m_is_set);
        return m_vector;
    }

    bool m_is_set = false;
    coefficients_vector m_vector;
};
}

/// Layer for storing the coefficients
template <class Super>
class layer_coefficients_storage : public Super
{
public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_stream.reset();
    }

    void push_symbol()
    {
        Super::push_symbol();

        m_stream.push(optional_coefficients_vector::unset());

        // Make sure we are in sync
        assert(Super::stream_lower_bound().value == m_stream.lower_bound());
        assert(Super::stream_upper_bound().value == m_stream.upper_bound());

        // Extend the existing vectors to make sure they can contain the
        // coefficients in the stream
        extend_vectors();
    }

    /// Pop a symbol from the stream
    auto pop_symbol() -> uint8_t*
    {
        uint8_t* symbol_data = Super::pop_symbol();
        m_stream.pop();

        // Make sure we are in sync
        assert(Super::stream_lower_bound().value == m_stream.lower_bound());
        assert(Super::stream_upper_bound().value == m_stream.upper_bound());

        return symbol_data;
    }

    /// Set the stream lower bound
    void set_stream_lower_bound(symbol_index stream_lower_bound)
    {
        Super::set_stream_lower_bound(stream_lower_bound);
        m_stream.set_lower_bound(stream_lower_bound.value);

        // Make sure we are in sync
        assert(Super::stream_lower_bound().value == m_stream.lower_bound());
        assert(Super::stream_upper_bound().value == m_stream.upper_bound());
    }

    /// Return the coefficients corresponding to the index
    auto coefficients_data(symbol_index index) const -> const_coefficients_view
    {
        assert(m_stream.in_stream(index.value));
        auto& item = m_stream[index.value];
        assert(item.is_set());
        return item.vector().to_const_view();
    }

    /// Return the coefficients corresponding to the index
    auto coefficients_data(symbol_index index) -> mutable_coefficients_view
    {
        assert(m_stream.in_stream(index.value));
        auto& item = m_stream[index.value];
        assert(item.is_set());
        return item.vector().to_mutable_view();
    }

    /// Return `true``if the coefficient vector for the symbol with`index`
    /// is available.
    auto has_vector(symbol_index index) -> bool
    {
        if (!m_stream.in_stream(index.value))
        {
            return false;
        }

        return m_stream[index.value].is_set();
    }

    /// Set the coefficient vector for the symbol
    void set_coefficients_vector(symbol_index index, coefficients_vector vector)
    {
        assert(m_stream.in_stream(index.value));
        assert(!has_vector(index));

        m_stream[index.value] = {true, std::move(vector)};
    }

protected:
    /// Make sure the coefficient vectors have sufficient memory to represent
    /// the stream.
    void extend_vectors()
    {
        auto new_upper_bound =
            Super::to_byte_index(Super::stream_frame().upper_bound());

        for (auto& v : m_stream)
        {
            if (!v.is_set())
            {
                continue;
            }

            v.vector().extend(new_upper_bound);
        }
    }

private:
    // List of pointers to the symbols in the stream
    kodo::slide::stream<optional_coefficients_vector> m_stream;
};
}
}
}
}
