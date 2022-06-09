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

#include <cstring>

#include <gtest/gtest.h>

#include <fifi/field/bytes_to_elements.hpp>
#include <fifi/field/elements_to_bytes.hpp>
#include <fifi/field/get_value.hpp>
#include <fifi/online/subtract.hpp>

#include <fifi/field/prime2325_apply_prefix.hpp>

#include "random_vector.hpp"

namespace helper
{

/// Test that we can find a prefix for in a randomly generated
/// buffer, given we choose the buffer length in such a way that
/// one unused prefix is guaranteed to exist.
///
/// @param data The random buffer to search for a prefix
template <class Algorithm>
inline void test_find_a_prefix(std::vector<uint8_t> data)
{
    Algorithm p(data.size());

    uint32_t prefix = p.find_prefix(data.data(), data.size());

    // Block elements
    uint32_t elements =
        fifi::field::bytes_to_elements(fifi::field::prime2325(), data.size());

    // Check that the prefix does not appear in the data
    for (uint32_t i = 0; i < elements; ++i)
    {
        uint32_t value = 0;
        std::memcpy(&value, data.data() + (i * sizeof(value)), sizeof(value));
        EXPECT_NE(value, prefix);
    }

    // Apply the prefix and test that all values are below the prime
    fifi::field::prime2325_apply_prefix(data.data(), data.size(), ~prefix);

    // Check that all values are below the prefix
    for (uint32_t i = 0; i < elements; ++i)
    {
        uint32_t value =
            fifi::field::get_value(fifi::field::prime2325(), data.data(), i);
        EXPECT_TRUE(value < fifi::field::prime2325().prime());
    }
}

/// Creates a random and deterministic block of data to search for
/// a prefix.
///
/// @param block_length The length of the block we want to test.
template <class Algorithm>
inline void test_find_a_prefix(uint32_t elements)
{
    std::size_t size =
        fifi::field::elements_to_bytes(fifi::field::prime2325(), elements);

    {
        std::vector<uint8_t> data(size);
        std::generate(data.begin(), data.end(), rand);

        test_find_a_prefix<Algorithm>(data);
    }

    {
        // We fill the buffer with 0xFF such that all the 32 bit integers will
        // be 0xFFFFFFFF which is above the prime.. the prefix should
        // remove those values
        std::vector<uint8_t> data(size, 0xFFU);
        test_find_a_prefix<Algorithm>(data);
    }
}

}
