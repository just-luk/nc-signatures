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
#include <cstring>
#include <list>
#include <vector>

#include <fifi/field/binary.hpp>
#include <fifi/field/prime2325.hpp>
#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>

#include "../finite_field.hpp"
#include "../version.hpp"

#include "ceil_division.hpp"
#include "floor_division.hpp"
#include "in_range.hpp"
#include "is_subrange.hpp"
#include "symbol_count.hpp"
#include "symbol_frame.hpp"
#include "symbol_range.hpp"
#include "to_byte_count.hpp"
#include "to_byte_index.hpp"
#include "to_symbol_count.hpp"
#include "to_symbol_frame.hpp"
#include "to_symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// This layer can convert between a bunch of different symbol index
/// mappings
template <class Super>
struct layer_mapping : public Super
{
public:
    template <class Range>
    auto is_subrange(const Range& a, const Range& b) const -> bool
    {
        return detail::is_subrange(a, b);
    }

    template <class Range>
    auto in_range(const Range& range,
                  const typename Range::value_type& value) const -> bool
    {
        return detail::in_range(range, value);
    }

    auto to_symbol_range(const symbol_frame& frame) const -> symbol_range
    {

        return detail::to_symbol_range(frame);
    }

    auto to_symbol_range(symbol_index lower_bound,
                         symbol_index upper_bound) const -> symbol_range
    {

        return detail::to_symbol_range(lower_bound, upper_bound);
    }

    auto to_symbol_range(symbol_index lower_bound, symbol_count symbols) const
        -> symbol_range
    {
        return detail::to_symbol_range(lower_bound, symbols);
    }

    template <class From>
    auto to_symbol_frame(const From& from) const -> symbol_frame
    {
        return detail::to_symbol_frame(Super::field_info(), from);
    }

    auto floor_division(std::size_t numerator, std::size_t denominator) const
        -> std::size_t
    {
        return detail::floor_division(numerator, denominator);
    }

    auto ceil_division(std::size_t numerator, std::size_t denominator) const
        -> std::size_t
    {
        return detail::ceil_division(numerator, denominator);
    }

    template <class From>
    auto to_byte_count(const From& from) const -> byte_count
    {
        return detail::to_byte_count(Super::field_info(), from);
    }
    template <class From>
    auto to_byte_index(const From& from) const -> byte_index
    {
        return detail::to_byte_index(Super::field_info(), from);
    }
};
}
}
}
