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

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <fifi/utils.hpp>

#include "random_value.hpp"

namespace helper
{
// Test helper returning a random vector in bytes respecting the math object
// granularity.
//
// @return size The size of vectors in bytes
inline auto random_vector(fifi::finite_field field, std::size_t size)
{
    std::vector<uint8_t> x(size);

    std::size_t elements = fifi::bytes_to_elements(field, size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        fifi::set_value(field, x.data(), i, random_value(field));
    }

    return x;
}
}
