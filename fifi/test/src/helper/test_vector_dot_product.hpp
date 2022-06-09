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
#include <fifi/online/multiply.hpp>
#include <fifi/utils.hpp>

#include "random_vector.hpp"

namespace helper
{
// Test helper calculating the vector dot product. First using the provided
// math implementation and then "by hand" invoking the basic add and multiply
// operations.
//
// @param math The math object to test
// @param field The finite field use for the computations
// @param size The size of vectors in bytes
// @param vectors The number of vectors to use in the dot product
template <class Math>
inline void test_vector_dot_product(const Math& math, fifi::finite_field field,
                                    std::size_t size, std::size_t vectors)
{
    std::size_t elements = fifi::bytes_to_elements(field, size);
    std::size_t constants_size = fifi::elements_to_bytes(field, vectors);

    std::vector<uint8_t> x = random_vector(field, vectors * size);
    std::vector<uint8_t> y = random_vector(field, vectors * size);
    std::vector<uint8_t> constants =
        random_vector(field, vectors * constants_size);

    std::vector<uint8_t*> x_ptr(vectors, nullptr);
    std::vector<const uint8_t*> y_ptr(vectors, nullptr);
    std::vector<const uint8_t*> constants_ptr(vectors, nullptr);

    for (std::size_t i = 0; i < vectors; ++i)
    {
        x_ptr[i] = x.data() + (i * size);
        y_ptr[i] = y.data() + (i * size);
        constants_ptr[i] = constants.data() + (i * constants_size);
    }

    math.vector_dot_product(x_ptr.data(), y_ptr.data(), constants_ptr.data(),
                            size, vectors, vectors);

    // For all destination vectors
    for (std::size_t i = 0; i < vectors; ++i)
    {
        // We look at each elemement
        for (std::size_t j = 0; j < elements; ++j)
        {
            // The operation on each element is:
            // a = b_0 * c_0 + b_1 * c_1 + ... b_k * c_k

            uint32_t sum = 0;

            for (std::size_t k = 0; k < vectors; ++k)
            {
                const uint8_t* y_k = y_ptr[k];
                const uint8_t* c_i = constants_ptr[i];

                uint32_t value_one = fifi::get_value(field, y_k, j);
                uint32_t value_two = fifi::get_value(field, c_i, k);

                sum = fifi::online::add(
                    field, sum,
                    fifi::online::multiply(field, value_one, value_two));
            }

            uint32_t actual = fifi::get_value(field, x_ptr[i], j);

            ASSERT_EQ(sum, actual);
        }
    }
}

/// This helper is used for standard math objects that have a field() function
template <class Math>
inline void test_vector_dot_product(const Math& math, std::size_t size,
                                    std::size_t vectors)
{
    test_vector_dot_product(math, math.field(), size, vectors);
}

}
