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
#include <cstring>
#include <list>
#include <vector>

#include "../version.hpp"

#include "byte_count.hpp"
#include "symbol_index.hpp"
#include "symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// Symbol storage
template <class Pointer, class Super>
struct layer_symbol_storage : public Super
{

    using symbol_storage_type = Pointer;

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_storage.clear();
        m_storage.resize(Super::symbols().value);

        m_symbols_set = symbol_count{0};
    }

    /// Set the storage of the block
    void set_symbols_storage(symbol_storage_type storage)
    {
        assert(storage != nullptr);
        assert(m_symbols_set.value == 0);

        std::size_t offset = 0;
        for (symbol_index index : Super::symbol_range())
        {
            layer_symbol_storage::set_symbol_storage(storage + offset, index);
            offset += Super::symbol_bytes().value;
        }
    }

    /// Set the storage of a symbol
    void set_symbol_storage(symbol_storage_type storage, symbol_index index)
    {
        assert(storage != nullptr);
        assert(index.value < Super::symbols().value);
        assert(!layer_symbol_storage::is_symbol_set(index));

        m_storage[index.value] = storage;
        ++m_symbols_set;
    }

    /// Copy src into the specified symbol
    void copy_into_symbol(symbol_index index, const uint8_t* src)
    {
        assert(src != nullptr);
        assert(index.value < Super::symbols().value);

        std::memcpy(symbol_data(index), src, Super::symbol_bytes().value);
    }

    /// Copy the specified symbol into dst
    void copy_from_symbol(symbol_index index, uint8_t* dst) const
    {
        assert(index.value < Super::symbols().value);
        assert(layer_symbol_storage::is_symbol_set(index));
        assert(dst != nullptr);

        std::memcpy(dst, layer_symbol_storage::symbol_data(index),
                    Super::symbol_bytes().value);
    }

    /// Return the symbol at the specified position
    auto symbol_data(symbol_index index) const -> symbol_storage_type
    {
        assert(index.value < Super::symbols().value);
        assert(layer_symbol_storage::is_symbol_set(index));
        return m_storage.at(index.value);
    }

    auto is_symbol_set(symbol_index index) const -> bool
    {
        assert(index.value < Super::symbols().value);
        return m_storage.at(index.value) != nullptr;
    }

    auto symbols_set() const -> symbol_count
    {
        return m_symbols_set;
    }

private:
    /// Storage for the symbols
    std::vector<symbol_storage_type> m_storage;
    symbol_count m_symbols_set;
};
}
}
}
