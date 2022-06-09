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

#include "binary16_basic.hpp"

#include "bytes_to_elements.hpp"
#include "get_value.hpp"
#include "set_value.hpp"

#include <cassert>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

constexpr char binary16_basic::name[];

uint32_t binary16_basic::add(uint32_t a, uint32_t b) const
{
    return a ^ b;
}

uint32_t binary16_basic::subtract(uint32_t a, uint32_t b) const
{
    return add(a, b);
}

uint32_t binary16_basic::multiply(uint32_t a, uint32_t b) const
{
    return m_table.multiply(a, b);
}

uint32_t binary16_basic::divide(uint32_t a, uint32_t b) const
{
    assert(b > 0);
    return multiply(invert(b), a);
}

uint32_t binary16_basic::invert(uint32_t a) const
{
    assert(a > 0U);

    return m_table.invert(a);
}

std::size_t binary16_basic::vector_multiply_add_into(uint8_t* x,
                                                     const uint8_t* y,
                                                     uint32_t constant,
                                                     std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert((size % 2) == 0);

    if (constant == 0)
    {
        return size;
    }
    else if (constant == 1)
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            x[i] = x[i] ^ y[i];
        }
    }
    else
    {
        std::size_t elements = bytes_to_elements(binary16(), size);

        for (std::size_t i = 0; i < elements; ++i)
        {
            // Here we calculate x[i] ^ (c * y[i])
            uint16_t x_value = get_value(binary16(), x, i);
            uint16_t y_value = get_value(binary16(), y, i);

            uint16_t result = x_value ^ multiply(y_value, constant);

            set_value(binary16(), x, i, result);
        }
    }

    return size;
}

std::size_t binary16_basic::vector_multiply_into(uint8_t* x, uint32_t constant,
                                                 std::size_t size) const
{
    assert(x != nullptr);
    assert(size > 0);

    if (constant == 0)
    {
        std::memset(x, 0, size);
    }
    else if (constant == 1)
    {
        return size;
    }
    else
    {
        std::size_t elements = bytes_to_elements(binary16(), size);

        for (std::size_t i = 0; i < elements; ++i)
        {
            // Here we calculate x[i] = c * x[i]
            uint16_t result = multiply(get_value(binary16(), x, i), constant);
            set_value(binary16(), x, i, result);
        }
    }

    return size;
}

std::size_t binary16_basic::vector_dot_product(uint8_t** x, const uint8_t** y,
                                               const uint8_t** constants,
                                               std::size_t size,
                                               std::size_t x_vectors,
                                               std::size_t y_vectors) const
{
    assert(x != nullptr);
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
            uint16_t constant = get_value(binary16(), constants[i], j);
            vector_multiply_add_into(x[i], y[j], constant, size);
        }
    }

    return size;
}

finite_field binary16_basic::field() const
{
    return fifi::finite_field::binary16;
}

binary16 binary16_basic::field_info() const
{
    return m_table.m_field;
}

bool binary16_basic::is_compiled()
{
    return true;
}

}
}
}
