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

#include "../../version.hpp"

#include "../symbol_index.hpp"

#include "outer_index.hpp"
#include "symbol_type.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class Super>
struct layer_symbol_mapping : public Super
{

    auto map_to_type(symbol_index index) const -> symbol_type
    {
        assert(index < Super::symbols());

        if (index < Super::zero_symbols())
        {
            // If the symbol index is in the zero padding range
            return symbol_type::zero;
        }

        // Remove the zero padding offset
        symbol_index relative_index = index - Super::zero_symbols();

        // How often do we have an outer symbol? The interval is the
        // number of data symbols between each outer symbol.
        //
        // E.g. if interval is 4 then we have "r0 s1 s2 s3 s4 r1 s5 ..." etc.
        symbol_count interval = Super::outer_interval();

        // The index offset between the repair symbols are then
        // 0, 5, 10 the data index is 1, 2, 3, 4, 6, 7 etc. Furthermore
        // we need to take into account the zero symbols
        if (interval == symbol_count{0})
        {
            return symbol_type::data;
        }

        // This means we have an outer symbol if the index evenly
        // divides interval + 1
        if ((relative_index.value % (interval.value + 1)) == 0U)
        {
            return symbol_type::outer;
        }

        // If we do not have zero or repair we have data
        return symbol_type::data;
    }

    /// Convert an outer symbol index to an inner/global symbol index
    auto to_outer_index(symbol_index index) const -> outer_index
    {
        assert(index < Super::symbols());
        assert(index >= Super::zero_symbols());
        assert(map_to_type(index) == symbol_type::outer);
        symbol_index relative_index = index - Super::zero_symbols();
        return outer_index{relative_index.value /
                           (Super::outer_interval().value + 1)};
    }

    /// Convert an outer symbol index to an inner/global symbol index
    auto to_symbol_index(outer_index index) const -> symbol_index
    {
        assert(index < Super::outer_symbols());
        return symbol_index{
            Super::zero_symbols().value +
            (index.value * (Super::outer_interval().value + 1))};
    }
};

}
}
}
}
