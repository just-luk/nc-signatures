// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>

#include "../version.hpp"
#include "prime2325.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// @param block_length The length of a block in 32 bit values values.
/// @return The length of the prefix in bits needed to ensure that given
///         the block length (i.e. the number of 32 bit values) there
///         exists a bit prefix not available in the data
inline std::size_t prime2325_prefix_length(std::size_t block_length)
{
    assert(block_length > 0);
    assert(block_length <= prime2325::max_block_length);

    // In the absence of a log2 function we look for the highest
    // top bit.
    uint32_t topbit;
    for (topbit = 31; topbit-- > 0;)
    {
        if (block_length >> topbit)
            break;
    }

    // How many values can be represented where the most
    // significant bit is top bit.  Create a mask of
    // 0000000..1111..1 where the MSBit is in the top bit position
    uint32_t prefix_values =
        ~(std::numeric_limits<uint32_t>::max() << topbit) + 1;

    if (prefix_values > block_length)
    {
        return topbit;
    }
    else
    {
        return topbit + 1;
    }
}

}
}
}
