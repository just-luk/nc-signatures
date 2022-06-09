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

#include <fifi/field/prime2325_binary_search.hpp>

#include <algorithm>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include "../helper/test_find_a_prefix.hpp"
#include "../helper/test_find_one_prefix.hpp"

TEST(test_prime2325_binary_search, find_one_prefix)
{
    helper::test_find_one_prefix<fifi::field::prime2325_binary_search>(7);
    helper::test_find_one_prefix<fifi::field::prime2325_binary_search>(8);
    helper::test_find_one_prefix<fifi::field::prime2325_binary_search>(9);
    helper::test_find_one_prefix<fifi::field::prime2325_binary_search>(10);

    uint32_t bits = (rand() % 20) + 1;
    helper::test_find_one_prefix<fifi::field::prime2325_binary_search>(bits);
}

/// Tests the size_needed() function for the prime2325_binary_search
TEST(test_prime2325_binary_search, binary_search_size_needed)
{
    using namespace fifi::field;

    // The size needed for the binary search depends on the
    // block length, the number of passes and the size of the
    // counter. In the below it is assumed that the counter is
    // uint32_t if this changes in the future the test has to
    // be updated

    std::size_t size_of_counter = sizeof(uint32_t);

    // The prefix length for 127 is 7 so space needed will be
    // 2^ceil(7/k_pass) * sizeof(uint32_t)
    EXPECT_EQ(prime2325_binary_search::size_needed(127, 1),
              128 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 2),
              16 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 3),
              8 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 4),
              4 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 5),
              4 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 6),
              4 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 7),
              2 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 8),
              2 * size_of_counter);

    EXPECT_EQ(prime2325_binary_search::size_needed(127, 20),
              2 * size_of_counter);
}

/// Find a prefix for different block sizes
TEST(test_prime2325_binary_search, find_a_prefix)
{
    helper::test_find_a_prefix<fifi::field::prime2325_binary_search>(7);
    helper::test_find_a_prefix<fifi::field::prime2325_binary_search>(8);
    helper::test_find_a_prefix<fifi::field::prime2325_binary_search>(9);
    helper::test_find_a_prefix<fifi::field::prime2325_binary_search>(512);

    uint32_t bits = (rand() % 200000) + 1;
    helper::test_find_a_prefix<fifi::field::prime2325_binary_search>(bits);
}
