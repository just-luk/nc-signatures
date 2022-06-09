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

#include <random>

namespace helper
{
/// Helper that returns a random number of symbols
inline auto random_symbols(std::size_t min_value = 1,
                           std::size_t max_value = 1000) -> std::size_t
{
    std::random_device random;
    std::uniform_int_distribution<> distribution(min_value, max_value);

    return static_cast<std::size_t>(distribution(random));
}

}