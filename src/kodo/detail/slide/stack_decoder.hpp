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
#include "../layer_reset.hpp"
#include "layer_allocate_coefficients_vector.hpp"
#include "layer_coefficients_storage.hpp"
#include "layer_coefficients_view_math.hpp"
#include "layer_common_symbol_storage.hpp"
#include "layer_decoder_symbol_storage.hpp"
#include "layer_decoding_algorithm.hpp"
#include "layer_decoding_operations.hpp"
#include "layer_decoding_status.hpp"
#include "layer_decoding_status_counter.hpp"
#include "layer_is_coefficients_decoded.hpp"
#include "layer_log_coefficients_storage.hpp"
#include "layer_log_decoding_algorithm.hpp"
#include "layer_mapping.hpp"
#include "layer_reset_symbols.hpp"
#include "layer_systematic_decoding_algorithm.hpp"

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
template<class Math>
using stack_decoder =
    layer_reset_symbols<
    layer_reset<
    layer_log_decoding_algorithm<
    layer_systematic_decoding_algorithm<
    layer_decoding_algorithm<
    layer_decoding_operations<
    layer_is_coefficients_decoded<
    layer_allocate_coefficients_vector<
    layer_decoding_status_counter<
    layer_decoding_status<
    layer_log_coefficients_storage<
    layer_coefficients_storage<
    layer_decoder_symbol_storage<
    layer_common_symbol_storage<uint8_t*,
    layer_coefficients_view_math<
    slide::layer_mapping<
    detail::layer_mapping<
    layer_math<Math,
    layer_log<
    layer_final>>>>>>>>>>>>>>>>>>>;
// clang-format on
}
}
}
}
