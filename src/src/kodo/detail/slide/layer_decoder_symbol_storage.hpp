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
#include "../to_symbol_frame.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// @brief Basic layer managing the symbol storage operations
template <class Super>
struct layer_decoder_symbol_storage : public Super
{
    using symbol_pointer_type = typename Super::symbol_pointer_type;

    /// Push a new symbol to the back of the stream.
    void push_symbol()
    {
        Super::push_symbol(nullptr, byte_count{0});
    }
};
}
}
}
}
