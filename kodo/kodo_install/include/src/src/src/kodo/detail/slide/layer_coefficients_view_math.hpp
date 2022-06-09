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

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../in_frame.hpp"
#include "../relative_index.hpp"
#include "../symbol_index.hpp"
#include "coefficients_vector.hpp"
#include "coefficients_view.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Finite field math operations for the coefficient and symbol views.
template <class Super>
struct layer_coefficients_view_math : public Super
{
    using Super::vector_multiply_subtract_into;
    /// Multiplies the `src` view with the `value` and subtracts it
    /// from the `dst` view. Both views should be `size` in bytes.
    void vector_multiply_subtract_into(const mutable_coefficients_view& dst,
                                       const const_coefficients_view& src,
                                       uint32_t value) const
    {
        // Adjust the views such that they have aligned lower bounds
        byte_index lower_bound = std::max(dst.lower_bound(), src.lower_bound());

        auto aligned_dst = Super::slide_view(lower_bound, dst);
        auto aligned_src = Super::slide_view(lower_bound, src);

        assert(aligned_dst.bytes() == aligned_src.bytes());
        assert(aligned_dst.lower_bound() == aligned_src.lower_bound());

        assert(aligned_dst != aligned_src);

        Super::vector_multiply_subtract_into(
            aligned_dst.data(), aligned_src.data(), value, aligned_dst.bytes());
    }

    using Super::vector_subtract_into;
    /// Multiplies the `src` view with the `value` and subtracts it
    /// from the `dst` view. Both views should be `size` in bytes.
    void vector_subtract_into(const mutable_coefficients_view& dst,
                              const const_coefficients_view& src) const
    {
        // Adjust the views such that they have aligned lower bounds
        byte_index lower_bound = std::max(dst.lower_bound(), src.lower_bound());

        auto aligned_dst = Super::slide_view(lower_bound, dst);
        auto aligned_src = Super::slide_view(lower_bound, src);

        assert(aligned_dst.bytes() == aligned_src.bytes());
        assert(aligned_dst.lower_bound() == aligned_src.lower_bound());

        assert(aligned_dst != aligned_src);

        Super::vector_subtract_into(aligned_dst.data(), aligned_src.data(),
                                    aligned_dst.bytes());
    }

    using Super::vector_multiply_into;
    /// Multiplies the `dst` view with the `value`.
    void vector_multiply_into(const mutable_coefficients_view& dst,
                              uint32_t value) const
    {
        Super::vector_multiply_into(dst.data(), value, dst.bytes());
    }

    /// Get the coefficient at `index` in the `from` view.
    uint32_t get_value(const const_coefficients_view& view,
                       symbol_index index) const
    {
        symbol_frame frame = Super::coefficients_view_to_symbol_frame(view);
        assert(in_frame(frame, index));
        return Super::get_value(view.data(), relative_index(frame, index));
    }

    void set_value(const coefficients_vector& vector, symbol_index index,
                   uint32_t value) const
    {
        set_value(vector.to_mutable_view(), index, value);
    }

    /// Set the coefficient at `index` in the `view`.
    void set_value(const mutable_coefficients_view& view, symbol_index index,
                   uint32_t value) const
    {
        symbol_frame frame = Super::coefficients_view_to_symbol_frame(view);
        assert(in_frame(frame, index));

        Super::set_value(view.data(), relative_index(frame, index), value);
    }
};
}
}
}
}
