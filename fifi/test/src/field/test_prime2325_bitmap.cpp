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

#include <fifi/field/prime2325_bitmap.hpp>

#include <algorithm>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <fifi/field/prime2325_apply_prefix.hpp>
#include <fifi/field/set_value.hpp>

#include "../helper/test_find_a_prefix.hpp"
#include "../helper/test_find_one_prefix.hpp"

using namespace fifi::field;

TEST(test_prime2325_bitmap, prefix_bitmap)
{
    // We make the vector large enough for 255 uint32_t values
    uint32_t block_size = 255 * 4;
    std::vector<uint8_t> data(block_size, 0);

    // Set some of the top 8 bits
    fifi::field::set_value(prime2325(), data.data(), 0, 0x01000000);
    fifi::field::set_value(prime2325(), data.data(), 1, 0x02000000);

    prime2325_bitmap p(block_size);

    EXPECT_EQ(p.m_mapping_bits, 32U);
    EXPECT_EQ(p.m_shift_prefix, 24U);

    uint32_t prefix = p.find_prefix((const uint8_t*)data.data(), data.size());

    // Our data contains three values
    EXPECT_NE(0x00000000U, prefix);
    EXPECT_NE(0x01000000U, prefix);
    EXPECT_NE(0x02000000U, prefix);
}

TEST(test_prime2325_bitmap, find_one_prefix)
{
    helper::test_find_one_prefix<prime2325_bitmap>(7);
    helper::test_find_one_prefix<prime2325_bitmap>(8);
    helper::test_find_one_prefix<prime2325_bitmap>(9);
    helper::test_find_one_prefix<prime2325_bitmap>(10);

    uint32_t bits = (rand() % 20) + 1;
    helper::test_find_one_prefix<prime2325_bitmap>(bits);
}

/// Tests the size_needed function for the prime2325_bitmap
TEST(test_prime2325_bitmap, size_needed_bitmap)
{
    // The size returned is in bytes. We need one bit
    // per possible field value. For a block length of 255
    // we need 8 bits that gives 256 different field values
    // that is 256 / 8 = 32
    EXPECT_EQ(prime2325_bitmap::size_needed(127), 16U);
    EXPECT_EQ(prime2325_bitmap::size_needed(128), 32U);
    EXPECT_EQ(prime2325_bitmap::size_needed(254), 32U);
    EXPECT_EQ(prime2325_bitmap::size_needed(255), 32U);

    // 512 = need 10 bits = 1024 different field values, this gives
    // 1024 / 8 = 128 bytes
    EXPECT_EQ(prime2325_bitmap::size_needed(512), 128U);
}

/// For different block sizes find a prefix
TEST(test_prime2325_bitmap, find_a_prefix)
{
    helper::test_find_a_prefix<prime2325_bitmap>(7);
    helper::test_find_a_prefix<prime2325_bitmap>(8);
    helper::test_find_a_prefix<prime2325_bitmap>(9);
    helper::test_find_a_prefix<prime2325_bitmap>(512);

    uint32_t bits = (rand() % 200000) + 1;
    helper::test_find_a_prefix<prime2325_bitmap>(bits);
}
