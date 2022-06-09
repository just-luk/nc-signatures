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

#include <cstdint>
#include <type_traits>

#include <bourne/json.hpp>

#include "../version.hpp"

#include "range.hpp"
#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
using symbol_range = range<symbol_index, struct _symbol_range>;

inline std::ostream& operator<<(std::ostream& out, const symbol_range& range)
{
    bourne::json json;
    json["symbol_range"]["lower_bound"] = range.lower_bound().value;
    json["symbol_range"]["upper_bound"] = range.upper_bound().value;

    return out << json.dump();
}
}
}
}
