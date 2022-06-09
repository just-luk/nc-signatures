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

#include <kodo/detail/slide/layer_decoding_operations.hpp>

#include <kodo/detail/symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    stub::function<symbol_frame(slide::const_coefficients_view)>
        coefficients_view_to_symbol_frame;

    stub::function<symbol_index()> stream_lower_bound;
    stub::function<void(symbol_index)> set_symbol_decoded;
    stub::function<void(symbol_index, byte_count)> update_symbol_bytes;
    stub::function<bool(symbol_index)> is_coefficients_decoded;
    stub::function<bool(symbol_index)> is_symbol_decoded;
    stub::function<bool(symbol_index)> is_symbol_missing;
    stub::function<bool(symbol_index)> is_symbol_partially_decoded;
    stub::function<slide::mutable_coefficients_view(symbol_index)>
        coefficients_data;
    stub::function<uint32_t(const slide::const_coefficients_view&,
                            symbol_index)>
        get_value;
    stub::function<uint8_t*(symbol_index)> symbol_data;
    stub::function<byte_count(symbol_index)> symbol_bytes;

    stub::function<bool()> is_binary;

    template <class... Args>
    void vector_multiply_subtract_into(Args...)
    {
    }

    template <class... Args>
    void vector_subtract_into(Args...)
    {
    }
};
}

TEST(detail_slide_test_layer_decoding_operations, test_backward_substitute)
{
    slide::layer_decoding_operations<dummy> stack;
    dummy& d = stack;

    d.coefficients_view_to_symbol_frame.set_return(
        symbol_frame{symbol_index{2}, symbol_index{5}});
    d.stream_lower_bound.set_return(symbol_index{0});
    d.is_binary.set_return(false);
    d.is_symbol_decoded.set_return(false);
    d.is_coefficients_decoded.set_return(false);
    d.is_symbol_missing.set_return(false);
    d.is_symbol_partially_decoded.set_return(true);
    d.coefficients_data.set_return(slide::mutable_coefficients_view{
        (uint8_t*)0xdeadbeef, byte_index{0}, byte_count{5}});
    d.get_value.set_return(1U);
    d.symbol_data.set_return((uint8_t*)0xdeadbeef);
    d.symbol_bytes.set_return(byte_count{12});

    stack.backward_substitute(symbol_index{3});

    // We should see the backward substitute happen for indices in
    // lower than the pivot.

    EXPECT_TRUE(d.coefficients_data.expect_calls()
                    .with(symbol_index{3})
                    .with(symbol_index{0U})
                    .with(symbol_index{1U})
                    .with(symbol_index{2U})
                    .to_bool());
}
