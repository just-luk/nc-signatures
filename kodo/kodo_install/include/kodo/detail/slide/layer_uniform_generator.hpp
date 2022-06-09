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

#include "../byte_count.hpp"
#include "../is_empty.hpp"
#include "../is_zero.hpp"
#include "../relative_index.hpp"
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

/// This layer generates the coding coefficients used for the encoding /
/// decoding a perpetual code with a fixed width.
///
/// It requires a layer underneath that supports generating coefficients
/// within a specific range.
///
template <class Super>
struct layer_uniform_generator : public Super
{
    /// Return the maximum number of bytes which will be generated
    auto coefficients_bytes(const symbol_range& range) const -> byte_count
    {
        return Super::to_byte_count(range);
    }

    /// @param data Pointer to the memory where the coding coefficients should
    ///        be generated.
    void generate(uint8_t* data, const symbol_range& range)
    {
        assert(data != nullptr);
        assert(!is_empty(range));

        auto bytes = coefficients_bytes(range);

        Super::generate_data(data, bytes);
        trim_data(data, range);
    }

private:
    /// Zeros coefficients outside the coding window.
    /// @param data Pointer to memory where the random coefficients are stored
    void trim_data(uint8_t* data, const symbol_range& range)
    {
        assert(data != nullptr);

        auto frame = Super::to_symbol_frame(range);

        auto view = Super::symbol_frame_to_coefficients_view(data, frame);

        for (symbol_index index : frame)
        {
            if (!range.in_range(index))
            {
                // Symbol coefficients in the frame but not in the range should
                // be 0.
                Super::set_value(view, index, 0U);
            }
            else if (!Super::is_binary() && Super::get_value(view, index) == 0U)
            {
                // Exclude the zero value for finite fields larger than binary
                Super::set_value(view, index, 1U);
            }
        }

        // Make sure the pivot is always set to 1
        Super::set_value(view, range.lower_bound(), 1U);
    }
};
}
}
}
}
