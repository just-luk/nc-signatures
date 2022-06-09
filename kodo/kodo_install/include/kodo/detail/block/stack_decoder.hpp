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

#include "../layer_decoding_status_counter.hpp"
#include "../layer_decoding_status_tracker.hpp"
#include "../layer_final.hpp"
#include "../layer_log.hpp"
#include "../layer_mapping.hpp"
#include "../layer_math.hpp"
#include "../layer_reset.hpp"
#include "../layer_symbol_bytes.hpp"
#include "../layer_symbol_storage.hpp"
#include "../layer_symbols.hpp"

#include "layer_coefficients_bytes.hpp"
#include "layer_coefficients_storage.hpp"
#include "layer_decoding_algorithm.hpp"
#include "layer_decoding_callback.hpp"
#include "layer_decoding_status_updater.hpp"
#include "layer_is_coefficients_decoded.hpp"
#include "layer_recoding_algorithm.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
// clang-format off
template<class Math>
using stack_decoder =
    layer_reset<
    // Decoder API
    layer_recoding_algorithm<
    layer_decoding_status_updater<
    layer_decoding_algorithm<
    layer_decoding_callback<
    layer_decoding_status_counter<
    layer_decoding_status_tracker<
    layer_is_coefficients_decoded<
    // Storage API
    layer_coefficients_storage<
    layer_coefficients_bytes<
    layer_symbol_storage<uint8_t*,
    layer_symbol_bytes<
    layer_symbols<
    // Finite Field API
    layer_mapping<
    layer_math<Math,
    // Log Layer
    layer_log<
    // Final Layer
    layer_final>>>>>>>>>>>>>>>>;
// clang-format on
}
}
}
}
