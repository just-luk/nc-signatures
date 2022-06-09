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

#include <kodo/detail/slide/layer_is_coefficients_decoded.hpp>

#include <kodo/detail/byte_count.hpp>
#include <kodo/detail/byte_index.hpp>
#include <kodo/detail/slide/coefficients_view.hpp>
#include <kodo/detail/symbol_count.hpp>
#include <kodo/detail/symbol_frame.hpp>
#include <kodo/detail/symbol_index.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy_layer
{
    stub::function<slide::const_coefficients_view(symbol_index)>
        coefficients_data;

    stub::function<symbol_frame(const slide::const_coefficients_view&)>
        coefficients_view_to_symbol_frame;

    stub::function<bool(const slide::const_coefficients_view&, symbol_index)>
        get_value;
};
}

TEST(detail_slide_test_layer_is_coefficients_decoded, not_decoded)
{
    slide::layer_is_coefficients_decoded<dummy_layer> stack;
    dummy_layer& d = stack;

    d.coefficients_data.set_return(slide::const_coefficients_view{
        (const uint8_t*)0xdeadbeef, byte_index{10}, byte_count{15}});

    d.coefficients_view_to_symbol_frame.set_return(
        symbol_frame{symbol_index{10}, symbol_index{15}});

    // Will be called with index 10, 11, 13, 14
    d.get_value.set_return(0U, 0U, 1U);

    EXPECT_FALSE(stack.is_coefficients_decoded(symbol_index{12}));
}

TEST(detail_slide_test_layer_is_coefficients_decoded, decoded)
{
    slide::layer_is_coefficients_decoded<dummy_layer> stack;
    dummy_layer& d = stack;

    d.coefficients_data.set_return(slide::const_coefficients_view{
        (const uint8_t*)0xdeadbeef, byte_index{10}, byte_count{15}});

    d.coefficients_view_to_symbol_frame.set_return(
        symbol_frame{symbol_index{10}, symbol_index{15}});

    // Will be called with index 10, 11, 13, 14
    d.get_value.set_return(0U, 0U, 0U, 0U);

    EXPECT_TRUE(stack.is_coefficients_decoded(symbol_index{12}));
}
