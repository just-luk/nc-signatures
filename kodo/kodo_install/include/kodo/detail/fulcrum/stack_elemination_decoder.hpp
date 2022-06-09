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

#include "../block/layer_coefficients_storage.hpp"
#include "../layer_decoding_status_counter.hpp"
#include "../layer_decoding_status_tracker.hpp"
#include "../layer_final.hpp"
#include "../layer_log.hpp"
#include "../layer_mapping.hpp"
#include "../layer_math.hpp"
#include "../layer_symbol_bytes.hpp"
#include "../layer_symbol_storage.hpp"
#include "../layer_symbols.hpp"
#include "layer_elemination_decoding.hpp"
#include "layer_elemination_offset.hpp"

#include "../../version.hpp"

#include <fifi/field/binary_math.hpp>

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

/// The elemination decoder is responsible for decoding the expansion created by
/// the outer encoder. Since all symbols belonging to the expansion are consumed
/// by the elemination decoder we can eliminate them before passing the encoded
/// symbol to the inner decoder.
// clang-format off
using stack_elemination_decoder =
    // Elemination Decoder API
    layer_elemination_decoding<
    layer_decoding_status_counter<
    layer_decoding_status_tracker<
    // Storage API
    block::layer_coefficients_storage<
    layer_elemination_offset<
    layer_symbol_storage<uint8_t*,
    layer_symbol_bytes<
    layer_symbols<
    // Finite Field API
    layer_mapping<
    layer_math<fifi::field::binary_math,
    // Log Layer
    layer_log<
    // Final Layer
    layer_final>>>>>>>>>>>;
// clang-format on
}
}
}
}
