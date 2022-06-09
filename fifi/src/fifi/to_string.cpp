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

#include <cassert>

#include "to_string.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
/// The finite fields
std::string to_string(fifi::finite_field field)
{
    std::string name;
    switch (field)
    {
    case fifi::finite_field::binary:
        name = "binary";
        break;
    case fifi::finite_field::binary4:
        name = "binary4";
        break;
    case fifi::finite_field::binary8:
        name = "binary8";
        break;
    case fifi::finite_field::binary16:
        name = "binary16";
        break;
    case fifi::finite_field::prime2325:
        name = "prime2325";
        break;
    default:
        assert(0 && "Unsupported field");
    };

    return name;
}
}
}
