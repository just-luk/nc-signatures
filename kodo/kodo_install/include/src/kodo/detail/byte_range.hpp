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

#include <ostream>
#include <type_traits>

#include <bourne/json.hpp>

#include "byte_index.hpp"
#include "range.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
using byte_range = range<byte_index, struct _byte_range>;

inline std::ostream& operator<<(std::ostream& out, const byte_range& range)
{
    bourne::json json;
    json["byte_range"]["lower_bound"] = range.lower_bound().value;
    json["byte_range"]["upper_bound"] = range.upper_bound().value;

    return out << json.dump();
}
}
}
}
