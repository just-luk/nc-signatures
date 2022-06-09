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

#include "binary4_basic.hpp"

#include <cassert>

#include "get_value.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{
constexpr char binary4_basic::name[];

uint32_t binary4_basic::add(uint32_t a, uint32_t b) const
{
    assert(a < 16U);
    assert(b < 16U);
    return a ^ b;
}

uint32_t binary4_basic::subtract(uint32_t a, uint32_t b) const
{
    assert(a < 16U);
    assert(b < 16U);
    return add(a, b);
}

uint32_t binary4_basic::multiply(uint32_t a, uint32_t b) const
{
    assert(a < 16U);
    assert(b < 16U);

    const uint8_t* row = m_multiply[a];
    return row[b];
}

uint32_t binary4_basic::divide(uint32_t a, uint32_t b) const
{
    assert(a < 16U);
    assert(b < 16U);
    assert(b > 0);

    return multiply(invert(b), a);
}

uint32_t binary4_basic::invert(uint32_t a) const
{
    assert(a > 0U);
    assert(a < 16U);
    return m_inverse[a];
}

std::size_t binary4_basic::vector_multiply_add_into(uint8_t* x,
                                                    const uint8_t* y,
                                                    uint32_t constant,
                                                    std::size_t size) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(size > 0);
    assert(constant < 16U);

    const uint8_t* row = m_multiply[constant];

    for (std::size_t i = 0; i < size; ++i)
    {
        x[i] = x[i] ^ row[y[i]];
    }

    return size;
}

std::size_t binary4_basic::vector_multiply_into(uint8_t* x, uint32_t constant,
                                                std::size_t size) const
{
    assert(x != nullptr);
    assert(constant < 16);
    assert(size > 0);

    const uint8_t* row = m_multiply[constant];

    for (std::size_t i = 0; i < size; ++i)
    {
        x[i] = row[x[i]];
    }

    return size;
}

std::size_t binary4_basic::vector_dot_product(uint8_t** x, const uint8_t** y,
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
            uint8_t constant = get_value(binary4(), constants[i], j);
            vector_multiply_add_into(x[i], y[j], constant, size);
        }
    }

    return size;
}

finite_field binary4_basic::field() const
{
    return fifi::finite_field::binary4;
}

binary4 binary4_basic::field_info() const
{
    return m_field;
}

bool binary4_basic::is_compiled()
{
    return true;
}
}
}
}
