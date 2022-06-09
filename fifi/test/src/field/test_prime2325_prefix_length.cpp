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

#include <gtest/gtest.h>

#include <fifi/field/prime2325_prefix_length.hpp>

TEST(test_prime2325_prefix_length, prime2325_prefix_length)
{
    using namespace fifi::field;

    // 2^y => 1 << y

    // In 8 bits we can write 256 distinct bit values so 8 bit should
    // be enough for a block length of 255 etc.
    EXPECT_EQ(prime2325_prefix_length(127), 7U);
    EXPECT_EQ(prime2325_prefix_length(128), 8U);
    EXPECT_EQ(prime2325_prefix_length(255), 8U);
    EXPECT_EQ(prime2325_prefix_length(256), 9U);
    EXPECT_EQ(prime2325_prefix_length(511), 9U);
    EXPECT_EQ(prime2325_prefix_length(512), 10U);
    EXPECT_EQ(prime2325_prefix_length((1 << 29) - 1), 29U);
}
