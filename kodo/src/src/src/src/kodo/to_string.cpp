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

#include "to_string.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
/// The finite fields
std::string to_string(finite_field field)
{
    std::string name;
    switch (field)
    {
    case finite_field::binary:
        name = "binary";
        break;
    case finite_field::binary4:
        name = "binary4";
        break;
    case finite_field::binary8:
        name = "binary8";
        break;
    case finite_field::binary16:
        name = "binary16";
        break;
    case finite_field::prime2325:
        name = "prime2325";
        break;
    default:
        assert(0 && "Unsupported field");
    };

    return name;
}

std::string to_string(perpetual::width width)
{
    std::string name;
    switch (width)
    {
    case perpetual::width::_8:
        name = "8";
        break;
    case perpetual::width::_16:
        name = "16";
        break;
    case perpetual::width::_32:
        name = "32";
        break;
    case perpetual::width::_64:
        name = "64";
        break;
    default:
        assert(0 && "Unsupported width");
    };

    return name;
}
}
}
