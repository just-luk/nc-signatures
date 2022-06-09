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

#include "../../finite_field.hpp"
#include "../../version.hpp"

#include "../symbol_frame.hpp"
#include "../to_byte_count.hpp"
#include "../to_symbol_frame.hpp"
#include "coefficients_view.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// This layer can convert between a bunch of different symbol index
/// mappings
template <class Super>
struct layer_mapping : public Super
{
public:
    template <class T>
    coefficients_view<T> slide_view(symbol_index lower_bound,
                                    const coefficients_view<T>& view) const
    {
        return slide_view(Super::to_byte_index(lower_bound), view);
    }

    template <class T>
    coefficients_view<T> slide_view(byte_index lower_bound,
                                    const coefficients_view<T>& view) const
    {
        // The new lower bound must be larger or equal to the current lower
        // bound we can only slide in the "increasing" byte number direction
        assert(lower_bound >= view.lower_bound());

        // The number of bytes we skip over
        byte_count skipped_bytes = lower_bound - view.lower_bound();

        // We cannot skip more than the size of the view
        assert(skipped_bytes < view.bytes());

        auto data = view.data() + skipped_bytes.value;
        auto bytes = view.bytes() - skipped_bytes;

        return coefficients_view<T>{data, lower_bound, bytes};
    }

    template <class T>
    symbol_frame
    coefficients_view_to_symbol_frame(const coefficients_view<T>& view) const
    {
        return Super::to_symbol_frame(view.range());
    }

    template <class T>
    coefficients_view<T>
    symbol_frame_to_coefficients_view(T data, const symbol_frame& frame) const
    {
        assert(data != nullptr);
        byte_index lower_bound = Super::to_byte_index(frame.lower_bound());
        byte_count size = Super::to_byte_count(frame);
        return coefficients_view<T>{data, lower_bound, size};
    }
};
}
}
}
}
