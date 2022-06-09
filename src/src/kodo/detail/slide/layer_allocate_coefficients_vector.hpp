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

#include <recycle/unique_pool.hpp>

#include "coefficients_vector.hpp"

#include "../byte_count.hpp"
#include "../symbol_frame.hpp"
#include "../symbol_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// The layer allocates a coefficient_vector use by the decoder to decode an
/// incoming symbol.
///
/// The incoming coefficients will be from the current coding window. During
/// elimination we may introduce non-zero coefficients for symbols currently
/// in the stream. So the coefficient vector we allocate must be large enough
/// to handle this.
template <class Super>
class layer_allocate_coefficients_vector : public Super
{
private:
    using memory_pool = coefficients_vector::memory_pool;

public:
    /// @return Allocate a vector for the coefficients of an incoming symbol
    auto allocate_coefficients_vector(symbol_range window)
        -> coefficients_vector
    {
        symbol_frame frame = Super::to_symbol_frame(window);

        // Lets get the size of the current window and the stream
        auto window_byte_lower_bound =
            Super::to_byte_index(frame.lower_bound());
        auto stream_byte_upper_bound =
            Super::to_byte_index(Super::stream_frame().upper_bound());

        // Now the size of the coefficients vector must be form the beginning of
        // the window to the end of the stream
        byte_count bytes = stream_byte_upper_bound - window_byte_lower_bound;

        // Lets allocate a vector
        assert(bytes > byte_count{0U});

        auto data = m_pool.allocate();
        data->resize(bytes.value);
        std::memset(data->data(), 0U, data->size());

        return coefficients_vector(window_byte_lower_bound, std::move(data));
    }

private:
    memory_pool m_pool;
};
}
}
}
}
