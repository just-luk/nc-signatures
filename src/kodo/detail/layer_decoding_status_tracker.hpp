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
#include <vector>

#include "../version.hpp"
#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// @brief The symbol decoding status tracker stores information about
///        the status of the different symbols contained within a decoder.
template <class Super>
class layer_decoding_status_tracker : public Super
{
public:
    /// Enumeration for the possible states of symbols during decoding
    enum class decoding_status
    {
        /// The symbol has not been received by the decoder
        missing,
        /// The symbol has been received, but it has not been fully decoded yet
        partially_decoded,
        /// The symbol has been received and fully decoded
        decoded
    };

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        m_status.resize(config.symbols.value);
        std::fill(m_status.begin(), m_status.end(), decoding_status::missing);
    }

    void set_symbol_missing(symbol_index index)
    {
        assert(index <= Super::last_symbol());
        m_status[index.value] = decoding_status::missing;
    }

    void set_symbol_partially_decoded(symbol_index index)
    {
        assert(index <= Super::last_symbol());
        m_status[index.value] = decoding_status::partially_decoded;
    }

    void set_symbol_decoded(symbol_index index)
    {
        assert(index <= Super::last_symbol());
        m_status[index.value] = decoding_status::decoded;
    }

    void set_symbols_decoded()
    {
        std::fill(m_status.begin(), m_status.end(), decoding_status::decoded);
    }

    bool is_symbol_missing(symbol_index index) const
    {
        assert(index <= Super::last_symbol());
        return m_status[index.value] == decoding_status::missing;
    }

    bool is_symbol_partially_decoded(symbol_index index) const
    {
        assert(index <= Super::last_symbol());
        return m_status[index.value] == decoding_status::partially_decoded;
    }

    bool is_symbol_decoded(symbol_index index) const
    {
        assert(index <= Super::last_symbol());
        return m_status[index.value] == decoding_status::decoded;
    }

    bool is_symbol_pivot(symbol_index index) const
    {
        assert(index <= Super::last_symbol());
        return is_symbol_partially_decoded(index) || is_symbol_decoded(index);
    }

public:
    /// Tracks the status of each symbol
    std::vector<decoding_status> m_status;
};
}
}
}
