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

#include "../../symbol_decoded_callback.hpp"
#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
/// The symbol decoding status updater checks if a symbols has been
/// fully decoded. This may mappend during the Gaussian elimination
/// process on the decoder.
template <class Super>
class layer_decoding_callback : public Super
{

public:
    template <class Config>
    void configure(Config& config)
    {
        Super::configure(config);

        m_callback = nullptr;
        m_user_data = nullptr;
    }

    void on_symbol_decoded(const kodo::symbol_decoded_callback& callback,
                           void* user_data)
    {
        m_callback = callback;
        m_user_data = user_data;
    }

    void set_symbol_decoded(symbol_index index)
    {
        Super::set_symbol_decoded(index);

        if (m_callback)
        {
            m_callback(index.value, m_user_data);
        }
    }

    void set_symbols_decoded()
    {
        // If the callback is set we to call it for all symbols that hasn't been
        // decoded yet
        if (m_callback)
        {
            for (auto index : Super::symbol_range())
            {
                if (!Super::is_symbol_decoded(index))
                {
                    set_symbol_decoded(index);
                }
            }
        }
        else
        {
            Super::set_symbols_decoded();
        }
    }

    auto is_on_symbol_decoded_set() const -> bool
    {
        return m_callback != nullptr;
    }

private:
    /// Callback that fires when a symbol is decoded
    kodo::symbol_decoded_callback m_callback = nullptr;

    /// User data to provide in callback
    void* m_user_data = nullptr;
};
}
}
}
}
