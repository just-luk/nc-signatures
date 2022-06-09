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

#include <fifi/field/binary.hpp>
#include <fifi/field/prime2325.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>

#include "../version.hpp"

#include "bit_index.hpp"
#include "byte_index.hpp"
#include "symbol_frame.hpp"
#include "symbol_index.hpp"
#include "symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

inline auto to_byte_index(fifi::field::binary, symbol_index index) -> byte_index
{
    return byte_index{static_cast<std::size_t>(index.value / 8U)};
}

inline auto to_byte_index(fifi::field::binary4, symbol_index index)
    -> byte_index
{
    return byte_index{static_cast<std::size_t>(index.value / 2U)};
}

inline auto to_byte_index(fifi::field::binary8, symbol_index index)
    -> byte_index
{
    return byte_index{static_cast<std::size_t>(index.value)};
}

inline auto to_byte_index(fifi::field::binary16, symbol_index index)
    -> byte_index
{
    return byte_index{static_cast<std::size_t>(index.value * 2)};
}

inline auto to_byte_index(fifi::field::prime2325, symbol_index index)
    -> byte_index
{
    return byte_index{static_cast<std::size_t>(index.value * 4)};
}

inline auto to_byte_index(bit_index bit) -> byte_index
{
    return byte_index{bit.value / 8U};
}

}
}
}
