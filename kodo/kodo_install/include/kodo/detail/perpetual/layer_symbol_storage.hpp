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

#include "symbol_type.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// Symbol storage for the perpetual code
///
template <class Pointer, class Super>
struct layer_symbol_storage : public Super
{

    // The configuration object
    struct config : public Super::config
    {
        byte_count symbol_bytes{0};
        byte_count block_bytes{0};
    };

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_symbol_bytes = config.symbol_bytes;
        m_block_bytes = config.block_bytes;

        assert(m_symbol_bytes > byte_count{0});
        assert(m_block_bytes > byte_count{0});

        byte_count zero_bytes = m_symbol_bytes * Super::zero_symbols();
        byte_count outer_bytes = m_symbol_bytes * Super::outer_symbols();

        m_zero.resize(zero_bytes.value, 0);
        m_outer.resize(outer_bytes.value, 0);

        m_zero_storage = m_zero.data();
        m_outer_storage = m_outer.data();
        m_data_storage = nullptr;

        assert(Super::map_to_type(Super::last_symbol()) == symbol_type::data);
    }

    /// Return the number of bytes allocated for each symbol
    auto max_symbol_bytes() const -> byte_count
    {
        assert(m_symbol_bytes > byte_count{0});

        return m_symbol_bytes;
    }

    /// Return the number of bytes allocated for each symbol
    auto symbol_bytes(symbol_index index) const -> byte_count
    {
        assert(index < Super::symbols());
        assert(m_symbol_bytes > byte_count{0});

        if (index < Super::last_symbol())
        {
            return m_symbol_bytes;
        }

        // This is the last symbol - so we have to calculate its size
        byte_count offset = index * m_symbol_bytes;
        byte_count total_bytes = byte_count{m_zero.size()} +
                                 byte_count{m_outer.size()} + m_block_bytes;

        assert(offset < total_bytes);

        return std::min(m_symbol_bytes, total_bytes - offset);
    }

    /// The number of bytes for the entire block
    auto block_bytes() const -> byte_count
    {
        assert(m_block_bytes > byte_count{0});
        return m_block_bytes;
    }

    /// Set the storage of the perpetual code
    void set_symbols_storage(Pointer storage)
    {
        assert(storage != nullptr);
        assert(m_block_bytes > byte_count{0});
        assert(m_symbol_bytes > byte_count{0});

        m_data_storage = storage;
    }

    /// Set the storage of the perpetual code
    auto symbols_storage() const -> Pointer
    {
        assert(m_data_storage != nullptr);
        return m_data_storage;
    }

    auto outer_storage() -> uint8_t*
    {
        assert(m_outer_storage != nullptr);
        return m_outer_storage;
    }

    /// Copy src into the specified symbol
    void copy_into_symbol(symbol_index index, const uint8_t* src)
    {
        assert(index < Super::symbols());
        assert(src != nullptr);

        std::memcpy(symbol_data(index), src, symbol_bytes(index).value);
    }

    /// Copy the specified symbol into dst
    void copy_from_symbol(symbol_index index, uint8_t* dst) const
    {
        assert(index < Super::symbols());
        assert(dst != nullptr);

        std::memcpy(dst, symbol_data(index), symbol_bytes(index).value);
    }

    /// Return the symbol at the specified position
    auto symbol_data(symbol_index index) const -> Pointer
    {
        assert(index < Super::symbols());
        assert(m_data_storage != nullptr);

        switch (Super::map_to_type(index))
        {
        case symbol_type::zero:
            assert(index < Super::zero_symbols());
            return m_zero_storage + (index.value * m_symbol_bytes.value);
        case symbol_type::outer:
            return m_outer_storage +
                   (Super::to_outer_index(index).value * m_symbol_bytes.value);
        case symbol_type::data:
            return data_symbol_data(index);

        default:
            assert(false);
            return nullptr;
        }
    }

private:
    /// Return the data symbol at the specified position
    auto data_symbol_data(symbol_index index) const -> Pointer
    {
        // Data symbols are interleaved with outer symbols. First we
        // remove the zero padding offset
        symbol_index relative_index = index - Super::zero_symbols();

        // We then calculate how many outer symbols we have interleaved
        // e.g. if the interval is 4 and we are looking at s5 then we have
        // r0 and r1 to remove
        symbol_count interval = Super::outer_interval();
        symbol_count repair_count = symbol_count{0};
        if (interval != symbol_count{0})
        {
            repair_count = symbol_count{
                (relative_index.value / (interval.value + 1)) + 1U};
        }
        // Remove the outer code offset
        return m_data_storage +
               ((relative_index - repair_count).value * m_symbol_bytes.value);
    }

private:
    /// The size in bytes for each symbol
    byte_count m_symbol_bytes;

    /// The storage block size
    byte_count m_block_bytes;

    /// Storage for the symbols
    Pointer m_data_storage = nullptr;

    /// Storage for the zero padding
    Pointer m_zero_storage = nullptr;

    /// Storage for the zero padding
    uint8_t* m_outer_storage = nullptr;

    /// Zero padding storage
    std::vector<uint8_t> m_zero;

    /// Outer code storage
    std::vector<uint8_t> m_outer;
};

}
}
}
}
