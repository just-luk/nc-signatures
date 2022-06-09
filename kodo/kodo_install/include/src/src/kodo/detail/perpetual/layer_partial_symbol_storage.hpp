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
#include <vector>

#include "../../version.hpp"

#include "../byte_count.hpp"
#include "../symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class Super>
struct layer_partial_symbol_storage : public Super
{
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        byte_count block_bytes = config.block_bytes;
        byte_count symbol_block_bytes =
            config.symbol_bytes * Super::data_symbols();

        assert(block_bytes > byte_count{0});
        assert(symbol_block_bytes >= block_bytes);

        m_partial_symbol_storage = nullptr;
        if (symbol_block_bytes > block_bytes)
        {
            m_partial_symbol.resize(config.symbol_bytes.value);
            std::fill(m_partial_symbol.begin(), m_partial_symbol.end(), 0);
            assert(m_partial_symbol.size() > 0);
            m_partial_symbol_storage = m_partial_symbol.data();
        }
    }

    auto has_partial_symbol() const -> bool
    {
        return m_partial_symbol_storage != nullptr;
    }

    void restore_partial_symbol()
    {
        assert(has_partial_symbol());
        Super::copy_into_symbol(Super::last_symbol(), m_partial_symbol_storage);
        Super::log("Partial symbol restored");
    }

    /// Return the number of bytes allocated for each symbol
    auto symbol_bytes(symbol_index index) const -> byte_count
    {
        assert(index < Super::symbols());

        if (has_partial_symbol() && (index == Super::last_symbol()))
        {
            return Super::max_symbol_bytes();
        }

        return Super::symbol_bytes(index);
    }

    /// Return the symbol at the specified position
    auto symbol_data(symbol_index index) const -> uint8_t*
    {
        assert(index < Super::symbols());

        if (has_partial_symbol() && (index == Super::last_symbol()))
        {
            return m_partial_symbol_storage;
        }
        else
        {
            return Super::symbol_data(index);
        }
    }

    void copy_into_symbol(symbol_index index, const uint8_t* src)
    {
        assert(index < Super::symbols());
        assert(src != nullptr);

        if (has_partial_symbol() && (index == Super::last_symbol()))
        {
            std::memcpy(m_partial_symbol_storage, src, m_partial_symbol.size());
        }
        else
        {
            return Super::copy_into_symbol(index, src);
        }
    }

    /// Copy the specified symbol into dst
    void copy_from_symbol(symbol_index index, uint8_t* dst) const
    {
        assert(index < Super::symbols());
        assert(dst != nullptr);

        if (has_partial_symbol() && (index == Super::last_symbol()))
        {
            std::memcpy(dst, m_partial_symbol_storage, m_partial_symbol.size());
        }
        else
        {
            return Super::copy_from_symbol(index, dst);
        }
    }

private:
    /// The storage for the partial symbol
    std::vector<uint8_t> m_partial_symbol;

    uint8_t* m_partial_symbol_storage = nullptr;
};
}
}
}
}
