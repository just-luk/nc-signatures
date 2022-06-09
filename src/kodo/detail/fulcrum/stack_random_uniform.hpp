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
#include "../layer_reset.hpp"
#include "../layer_symbols.hpp"

#include "../block/layer_uniform_generator.hpp"

#include "layer_coefficients_bytes.hpp"
#include "layer_expansion.hpp"
#include "layer_generate_recode.hpp"

#include <fifi/field/binary_math.hpp>

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{
// clang-format off
using stack_random_uniform =
    layer_reset<
    fulcrum::layer_generate_recode<
    block::layer_uniform_generator<
    layer_random_uniform<
    fulcrum::layer_coefficients_bytes<
    fulcrum::layer_expansion<
    layer_symbols<
    layer_mapping<
    layer_math<fifi::field::binary_math,
    layer_log<
    layer_final>>>>>>>>>>;
// clang-format on
}
}
}
}
