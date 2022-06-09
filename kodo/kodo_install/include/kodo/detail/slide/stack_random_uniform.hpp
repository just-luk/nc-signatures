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

#include "../layer_final.hpp"
#include "../layer_log.hpp"
#include "../layer_mapping.hpp"
#include "../layer_math.hpp"
#include "../layer_random_uniform.hpp"

#include "layer_coefficients_view_math.hpp"
#include "layer_log_generator.hpp"
#include "layer_mapping.hpp"
#include "layer_uniform_generator.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
// clang-format off
template <class Math>
using stack_random_uniform =
    // Generator API
    detail::slide::layer_log_generator<
    detail::slide::layer_uniform_generator<
    detail::layer_random_uniform<
    detail::slide::layer_coefficients_view_math<
    // Finite Field API
    detail::slide::layer_mapping<
    detail::layer_mapping<
    detail::layer_math<Math,
    // Log Layer
    detail::layer_log<
    // Final Layer
    detail::layer_final>>>>>>>>;
// clang-format on
}
}
}
}
