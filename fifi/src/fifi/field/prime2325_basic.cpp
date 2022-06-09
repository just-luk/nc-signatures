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

#include "prime2325_basic.hpp"

#include <algorithm>
#include <cassert>

#include "../online/add.hpp"
#include "../online/divide.hpp"
#include "../online/invert.hpp"
#include "../online/multiply.hpp"
#include "../online/subtract.hpp"
#include "../version.hpp"
#include "bytes_to_elements.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char prime2325_basic::name[];

uint32_t prime2325_basic::add(uint32_t a, uint32_t b) const
{
    return online::add(prime2325(), a, b);
}

uint32_t prime2325_basic::subtract(uint32_t a, uint32_t b) const
{
    return online::subtract(prime2325(), a, b);
}

uint32_t prime2325_basic::multiply(uint32_t a, uint32_t b) const
{
    return online::multiply(prime2325(), a, b);
}

uint32_t prime2325_basic::divide(uint32_t a, uint32_t b) const
{
    return online::divide(prime2325(), a, b);
}

uint32_t prime2325_basic::invert(uint32_t a) const
{
    return online::invert(prime2325(), a);
}

std::size_t prime2325_basic::vector_add_into(uint8_t* x, const uint8_t* y,
                                             std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        // Here we calculate x[i] = x[i] + y[i]
        uint32_t x_value = get_value(prime2325(), x, i);
        uint32_t y_value = get_value(prime2325(), y, i);

        uint32_t result = add(x_value, y_value);

        set_value(prime2325(), x, i, result);
    }

    return size;
}

std::size_t prime2325_basic::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                                  std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        // Here we calculate x[i] = x[i] - y[i]
        uint32_t x_value = get_value(prime2325(), x, i);
        uint32_t y_value = get_value(prime2325(), y, i);

        uint32_t result = subtract(x_value, y_value);

        set_value(prime2325(), x, i, result);
    }

    return size;
}

std::size_t prime2325_basic::vector_multiply_into(uint8_t* x, uint32_t constant,
                                                  std::size_t size) const
{
    assert(x != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        // Here we calculate x[i] = c * x[i]
        uint32_t result = multiply(get_value(prime2325(), x, i), constant);
        set_value(prime2325(), x, i, result);
    }

    return size;
}

std::size_t prime2325_basic::vector_multiply_add_into(uint8_t* x,
                                                      const uint8_t* y,
                                                      uint32_t constant,
                                                      std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        // Here we calculate x[i] + (c * y[i])
        uint32_t x_value = get_value(prime2325(), x, i);
        uint32_t y_value = get_value(prime2325(), y, i);

        uint32_t result = add(x_value, multiply(y_value, constant));

        set_value(prime2325(), x, i, result);
    }

    return size;
}

std::size_t prime2325_basic::vector_multiply_subtract_into(
    uint8_t* x, const uint8_t* y, uint32_t constant, std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(constant <= prime2325::max_value);
    assert(size > 0);

    std::size_t elements = bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        // Here we calculate x[i] - (c * y[i])
        uint32_t x_value = get_value(prime2325(), x, i);
        uint32_t y_value = get_value(prime2325(), y, i);

        uint32_t result = subtract(x_value, multiply(y_value, constant));

        set_value(prime2325(), x, i, result);
    }

    return size;
}

std::size_t prime2325_basic::vector_dot_product(uint8_t** x, const uint8_t** y,
                                                const uint8_t** constants,
                                                std::size_t size,
                                                std::size_t x_vectors,
                                                std::size_t y_vectors) const
{

    assert(x != nullptr);
    assert(y != nullptr);
    assert(constants != nullptr);
    assert(size > 0);
    assert(x_vectors > 0);
    assert(y_vectors > 0);

    for (std::size_t i = 0; i < x_vectors; ++i)
    {
        // Zero-initialize the destination buffer
        std::fill_n(x[i], size, 0);

        for (std::size_t j = 0; j < y_vectors; ++j)
        {
            // The vectors are multiplied and added into the result
            uint32_t constant = get_value(prime2325(), constants[i], j);
            vector_multiply_add_into(x[i], y[j], constant, size);
        }
    }

    return size;
}

finite_field prime2325_basic::field() const
{
    return fifi::finite_field::prime2325;
}

prime2325 prime2325_basic::field_info() const
{
    return prime2325();
}

bool prime2325_basic::is_compiled()
{
    return true;
}

}
}
}
