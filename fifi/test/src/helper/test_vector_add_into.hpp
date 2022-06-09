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

#include <algorithm>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include <fifi/online/add.hpp>
#include <fifi/utils.hpp>

#include "random_vector.hpp"

namespace helper
{
// Test helper calculating the vector add. First using the provided
// math implementation and then "by hand" using basic XOR operations.
//
// @param math The math object to test
// @param field The finite field use for the computations
// @param size The size of vectors in bytes
template <class Math>
inline void test_vector_add_into(const Math& math, std::size_t size)
{
    auto field = math.field();

    std::size_t elements = fifi::bytes_to_elements(field, size);

    std::vector<uint8_t> x = random_vector(field, size);
    std::vector<uint8_t> y = random_vector(field, size);
    // Keep a copy of the original x vector
    std::vector<uint8_t> x0(x);

    math.vector_add_into(x.data(), y.data(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        uint32_t value_x0 = fifi::get_value(field, x0.data(), i);
        uint32_t value_x = fifi::get_value(field, x.data(), i);
        uint32_t value_y = fifi::get_value(field, y.data(), i);

        EXPECT_EQ(fifi::online::add(field, value_x0, value_y), value_x);
    }
}
}
