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

#include "../symbol_index.hpp"

#include "../../slide/stream.hpp"
#include "../../symbol_decoded_callback.hpp"
#include "../../symbol_pivot_callback.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer for setting and getting the status of symbols.
template <class Super>
class layer_decoding_status : public Super
{
public:
    /// Defines the status that a symbol in a decoder can have.
    enum class status
    {
        /// missing means that the symbol has not yet been
        /// received by the decoder.
        missing,

        /// partially_decoded means that the symbol has been received,
        /// but it has not been fully decoded yet.
        partially_decoded,

        /// decoded means that the symbol has been received and
        /// fully decoded.
        decoded
    };

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_status.reset();
    }

    /// Push a symbol to the stream
    void push_symbol()
    {
        Super::push_symbol();
        m_status.push(status::missing);

        // Make sure we are in sync
        assert(Super::stream_lower_bound().value == m_status.lower_bound());
        assert(Super::stream_upper_bound().value == m_status.upper_bound());
    }

    /// Pop a symbol from the stream
    auto pop_symbol() -> uint8_t*
    {
        uint8_t* symbol_data = Super::pop_symbol();
        m_status.pop();

        // Make sure we are in sync
        assert(Super::stream_lower_bound().value == m_status.lower_bound());
        assert(Super::stream_upper_bound().value == m_status.upper_bound());

        return symbol_data;
    }

    /// Set the stream lower bound
    void set_stream_lower_bound(symbol_index stream_lower_bound)
    {
        Super::set_stream_lower_bound(stream_lower_bound);
        m_status.set_lower_bound(stream_lower_bound.value);
    }

    /// Set a specific symbol missing
    void set_symbol_missing(symbol_index index)
    {
        assert(m_status.in_stream(index.value));
        assert(!is_symbol_decoded(index));
        m_status[index.value] = status::missing;
    }

    /// Set a specific symbol partially decoded
    void set_symbol_partially_decoded(symbol_index index)
    {
        assert(m_status.in_stream(index.value));
        m_status[index.value] = status::partially_decoded;

        if (m_pivot_callback)
        {
            m_pivot_callback(index.value, m_pivot_user_data);
        }
    }

    /// Set a symbol fully decoded
    void set_symbol_decoded(symbol_index index)
    {
        assert(m_status.in_stream(index.value));

        if (m_status[index.value] != status::partially_decoded &&
            m_pivot_callback)
        {
            m_pivot_callback(index.value, m_pivot_user_data);
        }

        m_status[index.value] = status::decoded;

        if (m_decoded_callback)
        {
            m_decoded_callback(index.value, m_decoded_user_data);
        }
    }

    /// Set a symbol missing
    auto is_symbol_missing(symbol_index index) const -> bool
    {
        assert(m_status.in_stream(index.value));
        return m_status[index.value] == status::missing;
    }

    /// Return `true` if the symbol is partially decoded
    auto is_symbol_partially_decoded(symbol_index index) const -> bool
    {
        assert(m_status.in_stream(index.value));
        return m_status[index.value] == status::partially_decoded;
    }

    /// Return `true` if the symbol is fully decoded
    auto is_symbol_decoded(symbol_index index) const -> bool
    {
        assert(m_status.in_stream(index.value));
        return m_status[index.value] == status::decoded;
    }

    /// Return `true` if the symbol is pivot
    auto is_symbol_pivot(symbol_index index) const -> bool
    {
        assert(m_status.in_stream(index.value));
        return is_symbol_partially_decoded(index) || is_symbol_decoded(index);
    }

    void on_symbol_decoded(const kodo::symbol_decoded_callback& callback,
                           void* user_data)
    {
        m_decoded_callback = callback;
        m_decoded_user_data = user_data;
    }

    void on_symbol_pivot(const kodo::symbol_pivot_callback& callback,
                         void* user_data)
    {
        m_pivot_callback = callback;
        m_pivot_user_data = user_data;
    }

private:
    /// Tracks whether a symbol is still partially decoded
    kodo::slide::stream<status> m_status;

    /// Callback that fires when a symbol is decoded
    kodo::symbol_decoded_callback m_decoded_callback = nullptr;

    /// User data to provide in on_symbol_decoded callback
    void* m_decoded_user_data = nullptr;

    /// Callback that fires when a symbol is pivot
    kodo::symbol_pivot_callback m_pivot_callback = nullptr;

    /// User data to provide in on_symbol_pivot callback
    void* m_pivot_user_data = nullptr;
};
}
}
}
}
