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
class layer_decoding_status_updater : public Super
{
public:
    auto is_symbol_decoded(symbol_index index) const -> bool
    {
        if (Super::is_symbol_decoded(index))
        {
            return true;
        }

        if (Super::is_symbol_partially_decoded(index))
        {
            return Super::is_coefficients_decoded(index);
        }

        return false;
    }

    void decode_symbol(uint8_t* symbol, uint8_t* coefficients)
    {
        Super::decode_symbol(symbol, coefficients);

        if (Super::is_on_symbol_decoded_set())
        {
            update_symbol_status();
        }
    }

private:
    /// Go through all the symbols in the stream and check if any of
    /// them needs to be updated
    void update_symbol_status()
    {
        for (auto index : Super::symbol_range())
        {
            if (!Super::is_symbol_decoded(index))
            {
                if (Super::is_symbol_partially_decoded(index))
                {
                    if (Super::is_coefficients_decoded(index))
                    {
                        Super::set_symbol_decoded(index);
                    }
                }
            }
        }
    }
};
}
}
}
}
