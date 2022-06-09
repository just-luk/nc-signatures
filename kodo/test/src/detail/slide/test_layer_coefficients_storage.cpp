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

#include <kodo/detail/slide/layer_coefficients_storage.hpp>

#include <kodo/detail/to_byte_index.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy_layer
{
    // set_stream_lower_bound
    template <class Config>
    void configure(Config&)
    {
    }
    byte_index to_byte_index(symbol_index i) const
    {
        return kodo::detail::to_byte_index(fifi::field::binary8{}, i);
    }

    stub::function<void()> push_symbol;
    stub::function<uint8_t*()> pop_symbol;
    stub::function<symbol_frame()> stream_frame;
    stub::function<symbol_index()> stream_lower_bound;
    stub::function<symbol_index()> stream_upper_bound;
};
}

TEST(slide_detail_layer_coefficients_storage, push_pop)
{
    slide::layer_coefficients_storage<dummy_layer> stack;
    dummy_layer& dummy = stack;

    dummy.stream_frame.set_return(
        symbol_frame{symbol_index{0}, symbol_index{0U}});
    dummy.pop_symbol.set_return((uint8_t*)0xffaabbcc);

    // Before push set expectation
    dummy.stream_lower_bound.set_return(symbol_index{0U});
    dummy.stream_upper_bound.set_return(symbol_index{1U});

    stack.push_symbol();

    // Before pop set expectation
    dummy.stream_lower_bound.set_return(symbol_index{1U});
    dummy.stream_upper_bound.set_return(symbol_index{1U});

    uint8_t* symbol = stack.pop_symbol();
    EXPECT_EQ(symbol, (uint8_t*)0xffaabbcc);
}
