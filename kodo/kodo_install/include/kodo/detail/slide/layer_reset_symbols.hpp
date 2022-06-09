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

#include "../../slide/reset_callback.hpp"
#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
template <class Super>
class layer_reset_symbols : public Super
{
public:
    void reset(kodo::slide::reset_callback callback, void* user_data)
    {
        if (callback != nullptr)
        {
            for (auto index : Super::stream_range())
            {
                if (Super::is_symbol_initialized(index))
                {
                    callback(index.value, Super::symbol_data(index), user_data);
                }
            }
        }
        Super::reset();
    }
};
}
}
}
}
