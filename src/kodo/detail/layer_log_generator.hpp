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

#include <sstream>
#include <string>
#include <vector>

#include "../version.hpp"
#include "to_string.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

template <class Super>
struct layer_log_generator : public Super
{
    using field_type = typename Super::field_type;

    void generate(uint8_t* data)
    {
        Super::generate(data);

        auto range = Super::generator_range();
        Super::log("generate: ", log_coefficients<field_type>{data, range});
    }
};

}
}
}
