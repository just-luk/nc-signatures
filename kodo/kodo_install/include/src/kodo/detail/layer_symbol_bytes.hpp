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

#include "byte_count.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// Symbol bytes info
template <class Super>
struct layer_symbol_bytes : public Super
{

    // The configuration object
    struct config : public Super::config
    {
        byte_count symbol_bytes{0};
    };

    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_symbol_bytes = config.symbol_bytes;
        assert(m_symbol_bytes > byte_count{0});
    }

    /// Return the number of bytes allocated for each symbol
    auto symbol_bytes() const -> byte_count
    {
        assert(m_symbol_bytes > byte_count{0});
        return m_symbol_bytes;
    }

    /// The number of bytes for the entire block
    auto block_bytes() const -> byte_count
    {
        assert(m_symbol_bytes > byte_count{0});
        return byte_count{static_cast<std::size_t>(Super::symbols().value *
                                                   m_symbol_bytes.value)};
    }

private:
    /// The size in bytes for each symbol
    byte_count m_symbol_bytes;
};
}
}
}
