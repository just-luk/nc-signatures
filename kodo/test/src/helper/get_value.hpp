// License for Commercial Usage
// Distributed under the "KODO EVALUATION LICENSE 1.3"
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
// Distributed under the "KODO RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include <kodo/finite_field.hpp>

#include <fifi/utils.hpp>

#include <cassert>

namespace helper
{
/// Helper that returns a specific coefficient from a data buffer
inline uint32_t get_value(kodo::finite_field field, const uint8_t* elements,
                          std::size_t index)
{
    switch (field)
    {
    case kodo::finite_field::binary:
        return fifi::get_value(fifi::finite_field::binary, elements, index);
    case kodo::finite_field::binary4:
        return fifi::get_value(fifi::finite_field::binary4, elements, index);
    case kodo::finite_field::binary8:
        return fifi::get_value(fifi::finite_field::binary8, elements, index);
    case kodo::finite_field::binary16:
        return fifi::get_value(fifi::finite_field::binary16, elements, index);
    case kodo::finite_field::prime2325:
        return fifi::get_value(fifi::finite_field::prime2325, elements, index);

    default:
        assert(false);
        return 0;
    }
}

}