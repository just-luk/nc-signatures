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
#include "../layer_reset.hpp"
#include "../layer_symbol_bytes.hpp"
#include "../layer_symbols.hpp"

#include "layer_coefficients_storage.hpp"
#include "layer_decoders.hpp"
#include "layer_decoding_algorithm.hpp"
#include "layer_expansion.hpp"
#include "layer_recoding_algorithm.hpp"

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
template<class Math>
using stack_decoder =
    layer_reset<
    // Encoder API
    fulcrum::layer_recoding_algorithm<
    fulcrum::layer_decoding_algorithm<
    fulcrum::layer_coefficients_storage<
    fulcrum::layer_decoders<Math,
    // Symbol Storage API
    fulcrum::layer_expansion<
    layer_symbol_bytes<
    layer_symbols<
    // Log Layer
    layer_log<
    // Final Layer
    layer_final>>>>>>>>>;
// clang-format on
}
}
}
}
