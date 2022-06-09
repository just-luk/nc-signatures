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

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../../slide/stream.hpp"
#include "../in_range.hpp"
#include "../is_empty.hpp"
#include "../symbol_frame.hpp"
#include "../symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// @brief Common layer managing the symbol storage operations
template <class Pointer, class Super>
struct layer_common_symbol_storage : public Super
{
    using symbol_pointer_type = Pointer;

    /// @brief The config class
    struct config : public Super::config
    {
        /// The maximum size of symbols in bytes
        byte_count max_symbol_bytes;
    };

    /// @brief The storage of a symbol
    struct storage
    {
        /// Pointer to symbol data
        symbol_pointer_type data{nullptr};

        /// Size of symbol
        byte_count bytes;
    };

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_max_symbol_bytes = config.max_symbol_bytes;
        assert(m_max_symbol_bytes != byte_count{0});
        m_stream.reset();
        update();
    }

    /// Push a new symbol to the front of the stream.
    /// @param symbol The symbol data.
    /// @param symbol_bytes The symbol size.
    void push_symbol(symbol_pointer_type symbol, byte_count symbol_bytes)
    {
        assert(symbol_bytes <= m_max_symbol_bytes);
        m_stream.push({symbol, symbol_bytes});
        update();
    }

    /// Pop/remove the "oldest" symbol in the stream
    /// @return The pointer to the popped symbol.
    auto pop_symbol() -> symbol_pointer_type
    {
        assert(!m_stream.is_empty());
        auto front_symbol = m_stream.front();
        m_stream.pop();
        update();
        return front_symbol.data;
    }

    /// Set the stream lower bound
    /// @param stream_lower_bound The nwe stream lower bound.
    void set_stream_lower_bound(symbol_index stream_lower_bound)
    {
        m_stream.set_lower_bound(stream_lower_bound.value);
        update();
    }

    /// Sets the symbol storage for a specific symbol.
    /// @param index The index of the symbol we are setting
    /// @param symbol The data pointer of the symbol
    void set_symbol_storage(symbol_index index, symbol_pointer_type symbol_data,
                            byte_count symbol_bytes)
    {
        assert(m_stream.in_stream(index.value));
        assert(symbol_data != nullptr);
        assert(symbol_bytes > byte_count{0});
        assert(symbol_bytes <= m_max_symbol_bytes);
        m_stream[index.value].data = symbol_data;
        m_stream[index.value].bytes = symbol_bytes;
    }

    /// Updates the size of a previously initialized symbol. The new size must
    /// be less than the previous size.
    /// @param index The index of the symbol byte count we are setting
    /// @param symbol_bytes The new symbol byte count
    void update_symbol_bytes(symbol_index index, byte_count symbol_bytes)
    {
        assert(is_symbol_initialized(index));
        assert(symbol_bytes > byte_count{0});
        assert(symbol_bytes <= m_max_symbol_bytes);
        m_stream[index.value].bytes = symbol_bytes;
    }

    /// Return true of the symbol is in the stream range
    auto in_stream(symbol_index index) const -> bool
    {
        return in_range(m_range, index);
    }

    /// Return the range corresponding to the stream
    auto stream_range() const -> symbol_range
    {
        return m_range;
    }

    auto stream_lower_bound() const -> symbol_index
    {
        return symbol_index{m_stream.lower_bound()};
    }

    auto stream_upper_bound() const -> symbol_index
    {
        return symbol_index{m_stream.upper_bound()};
    }

    auto is_stream_empty() const -> bool
    {
        return m_stream.is_empty();
    }

    auto stream_symbols() const -> symbol_count
    {
        return symbol_count{m_stream.size()};
    }

    /// Return the frame corresponding to the stream
    auto stream_frame() const -> symbol_frame
    {
        return m_frame;
    }

    /// @return The front symbol
    auto front_symbol() const -> symbol_pointer_type
    {
        assert(!is_empty(m_range));
        return symbol_data(m_range.lower_bound());
    }

    /// @param index The index of the symbol we wish to access
    /// @return The data of the symbol.
    auto symbol_data(symbol_index index) const -> symbol_pointer_type
    {
        assert(is_symbol_initialized(index));
        return m_stream[index.value].data;
    }

    /// @param index The index of the symbol we wish to access
    /// @return The size of the symbol.
    auto symbol_bytes(symbol_index index) const -> byte_count
    {
        assert(is_symbol_initialized(index));
        return m_stream[index.value].bytes;
    }

    /// @return The maximum symbol size
    auto max_symbol_bytes() const -> byte_count
    {
        return m_max_symbol_bytes;
    }

    auto is_symbol_initialized(symbol_index index) const -> bool
    {
        assert(m_stream.in_stream(index.value));
        auto symbol = m_stream.at(index.value);
        return symbol.data != nullptr && symbol.bytes != byte_count{0};
    }

private:
    /// Update the internal state of the symbol storage
    void update()
    {
        m_range = Super::to_symbol_range(symbol_index{m_stream.lower_bound()},
                                         symbol_count{m_stream.size()});

        m_frame = Super::to_symbol_frame(m_range);
    }

private:
    /// List of pointers to the symbols in the stream
    kodo::slide::stream<storage> m_stream;

    /// The range of the stream
    symbol_range m_range;

    /// The frame of the stream
    symbol_frame m_frame;

    // The maximum size of a symbol
    byte_count m_max_symbol_bytes;
};
}
}
}
}
