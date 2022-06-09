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
#include "../layer_math.hpp"
#include "../layer_reset.hpp"

#include "layer_info.hpp"
#include "layer_inner_encoder.hpp"
#include "layer_outer_encoder.hpp"
#include "layer_outer_encoder_storage.hpp"
#include "layer_outer_generator.hpp"
#include "layer_symbol_mapping.hpp"
#include "layer_symbol_storage.hpp"

#include <fifi/field/binary_math.hpp>

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
// clang-format off
template<class InnerCoefficientsType>
using stack_encoder =
    layer_reset<
    // Encoder API
    layer_outer_encoder<
    layer_outer_encoder_storage<
    layer_outer_generator<
    layer_inner_encoder<
    // Storage API
    layer_symbol_storage<const uint8_t*,
    layer_symbol_mapping<
    layer_info<InnerCoefficientsType,
    // Finite Field API
    layer_math<fifi::field::binary_math,
    // Log Layer
    layer_log<
    // Final Layer
    layer_final>>>>>>>>> >;
// clang-format on
}
}
}
}
