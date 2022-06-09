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

#include <gtest/gtest.h>

#include <fifi/field/set_value.hpp>
#include <fifi/online/subtract.hpp>

#include <fifi/field/prime2325_prefix_length.hpp>

namespace helper
{

/// Test that we can find a prefix for a carefully generated
/// buffer. The values in the buffer are constructed so that only
/// one unique prefix is guaranteed to exist.
///
/// @param prefix_bits The number of prefix bits to use. This
/// determines the maximum block length that we can use.
template <class Algorithm>
inline void test_find_one_prefix(uint32_t prefix_bits)
{
    // If we have x bits then 2^x gives us the possible
    // values then we make block length 2^x - 1, then there
    // is exactly one missing prefix
    uint32_t block_length = (1 << prefix_bits) - 1;
    uint32_t block_size = block_length * 4U;

    std::vector<uint8_t> data(block_size);

    // Pick a value to leave unused
    uint8_t skip_value = rand() % block_length;

    // Number of bits needed in the prefix to ensure that at least
    // one value is unused.
    uint32_t prefix_length = fifi::field::prime2325_prefix_length(block_length);

    EXPECT_EQ(prefix_bits, prefix_length);

    uint32_t shift_prefix = 32 - prefix_length;

    // Here we fill the block of data with a unique value each
    // element making sure that only one prefix exist in the
    // block.
    //
    // This is done in the following way with the shift_prefix we
    // know which bits will be inspected by the prefix search so
    // we shift a number [0:block_length-1] up to the prefix
    //
    // Example (shift_prefix 28) of how the bits are filled:
    //
    // Iteration 0: 0000xxxxxxxxxx
    // Iteration 1: 0001xxxxxxxxxx
    // Iteration 2: 0010xxxxxxxxxx
    // Iteration 3: 0011xxxxxxxxxx
    //
    // and so forth.

    for (uint32_t i = 0; i < block_length; ++i)
    {
        if (i >= skip_value)
        {
            uint32_t value = (i + 1) << shift_prefix;
            fifi::field::set_value(fifi::field::prime2325(), data.data(), i,
                                   value);
        }
        else
        {
            uint32_t value = i << shift_prefix;
            fifi::field::set_value(fifi::field::prime2325(), data.data(), i,
                                   value);
        }
    }

    uint32_t missing_prefix = skip_value << shift_prefix;

    Algorithm p(block_size);

    uint32_t prefix = p.find_prefix(data.data(), data.size());

    EXPECT_EQ(missing_prefix, prefix);
}

}
