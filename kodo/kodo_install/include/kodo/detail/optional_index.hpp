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

#include <cassert>
#include <cstdint>

#include <bourne/json.hpp>

#include "../version.hpp"

#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

struct optional_index
{
    static optional_index not_found()
    {
        return {false, symbol_index{0}};
    }

    optional_index(bool found, symbol_index index) :
        m_found(found), m_index(index)
    {
    }

    auto found() const -> bool
    {
        return m_found;
    }

    auto index() const -> symbol_index
    {
        assert(m_found);
        return m_index;
    }

    bool m_found = false;
    symbol_index m_index;
};

inline auto operator==(const optional_index& a, const optional_index& b) -> bool
{
    return (a.found() == b.found()) && (a.index() == b.index());
}

inline auto operator<<(std::ostream& out, const optional_index& index)
    -> std::ostream&
{
    bourne::json json;
    json["optional_index"]["found"] = index.m_found;
    json["optional_index"]["index"] = index.m_index.value;

    return out << json.dump();
}

}
}
}
