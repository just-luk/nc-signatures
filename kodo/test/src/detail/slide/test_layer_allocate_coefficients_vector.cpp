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

#include <kodo/detail/slide/layer_allocate_coefficients_vector.hpp>

#include <kodo/detail/byte_index.hpp>
#include <kodo/detail/symbol_frame.hpp>
#include <kodo/detail/symbol_index.hpp>
#include <kodo/detail/to_byte_index.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    stub::function<symbol_frame(symbol_range)> to_symbol_frame;
    stub::function<symbol_frame()> stream_frame;
    byte_index to_byte_index(symbol_index i) const
    {
        return kodo::detail::to_byte_index(fifi::field::binary8{}, i);
    }
};
}

TEST(detail_slide_test_layer_allocate_coefficients_vector, api)
{
    slide::layer_allocate_coefficients_vector<dummy> stack;
    dummy& d = stack;
    d.to_symbol_frame.set_return(
        symbol_frame{symbol_index{2U}, symbol_index{3U}});
    d.stream_frame.set_return(symbol_frame{symbol_index{2U}, symbol_index{4U}});

    symbol_range window{symbol_index{2U}, symbol_index{3U}};

    auto vector_out = stack.allocate_coefficients_vector(window);

    EXPECT_EQ(vector_out.lower_bound(), byte_index{2U});
    EXPECT_EQ(vector_out.bytes(), byte_count{2U});
}
