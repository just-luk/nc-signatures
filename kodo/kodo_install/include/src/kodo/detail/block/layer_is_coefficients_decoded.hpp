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
/// @brief The symbol decoding status updater checks if a symbols has been
///        fully decoded. This may mappend during the Gaussian elimination
///        process on the decoder.
template <class Super>
class layer_is_coefficients_decoded : public Super
{
public:
    /// @return True if the symbol at the given index is decode.
    /// The underlying layers decoding state layers does not detect if a symbol
    /// have become decoded due to decoding operations.
    /// Therefore any symbol marked "partially decoded" can infact be fully
    /// decoded. This functions detects if this is the case.
    auto is_coefficients_decoded(symbol_index index) const -> bool
    {
        auto coefficients = Super::coefficients_data(index);
        auto range = Super::symbol_range();

        for (symbol_index i : range)
        {
            if (i == index)
            {
                // Skip the symbols pivot postion
                continue;
            }

            if (Super::get_value(coefficients, i) != 0U)
            {
                return false;
            }
        }
        return true;
    }
};
}
}
}
}
