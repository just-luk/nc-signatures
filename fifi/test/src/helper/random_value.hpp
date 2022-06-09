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

#include <fifi/finite_field.hpp>
#include <fifi/utils.hpp>

namespace helper
{

template <class Field>
inline uint32_t random_value(Field)
{
    return rand() % Field::order;
}

// Test helper returning a random value for the given finite field
inline uint32_t random_value(fifi::finite_field field)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return rand() % fifi::field::binary::order;
    case fifi::finite_field::binary4:
        return rand() % fifi::field::binary4::order;
    case fifi::finite_field::binary8:
        return rand() % fifi::field::binary8::order;
    case fifi::finite_field::binary16:
        return rand() % fifi::field::binary16::order;
    case fifi::finite_field::prime2325:
        return rand() % fifi::field::prime2325::order;
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}
}
