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

#include <cassert>

#include "to_field.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
/// The finite fields
finite_field to_field(const std::string& value)
{
    if (value == "binary")
    {
        return finite_field::binary;
    }
    else if (value == "binary4")
    {
        return finite_field::binary4;
    }
    else if (value == "binary8")
    {
        return finite_field::binary8;
    }
    else if (value == "binary16")
    {
        return finite_field::binary16;
    }
    else if (value == "prime2325")
    {
        return finite_field::prime2325;
    }
    else
    {
        assert(0 && "Unknown field");
        return finite_field::binary;
    }
}
}
}
