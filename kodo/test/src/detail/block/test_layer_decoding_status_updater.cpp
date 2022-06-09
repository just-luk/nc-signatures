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

#include <kodo/detail/block/layer_decoding_status_updater.hpp>

#include <kodo/detail/symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

namespace
{
struct dummy
{
    stub::function<bool(kodo::detail::symbol_index)> is_symbol_decoded;
    stub::function<bool(kodo::detail::symbol_index)>
        is_symbol_partially_decoded;
    stub::function<bool(kodo::detail::symbol_index)> is_coefficients_decoded;
    stub::function<void(uint8_t*, uint8_t*)> decode_symbol;
    stub::function<kodo::detail::symbol_range()> symbol_range;
    stub::function<void(kodo::detail::symbol_index)> set_symbol_decoded;
    stub::function<bool()> is_on_symbol_decoded_set;
};
}

TEST(detail_block_test_layer_decoding_status_updater, decode)
{

    using namespace kodo::detail;

    block::layer_decoding_status_updater<dummy> stack;
    dummy& d = stack;

    d.is_on_symbol_decoded_set.set_return(true);

    d.symbol_range.set_return(symbol_range{symbol_index{0}, symbol_index{5}});

    // Checked for index  0, 1, 2, 3, 4
    d.is_symbol_decoded.set_return(false, false, false, false, false);

    // Checked for index 0, 1, 2, 3, 4
    d.is_symbol_partially_decoded.set_return(true, false, true, false, true);

    // Checked for index 0, 2, 4
    d.is_coefficients_decoded.set_return(true, false, true);

    stack.decode_symbol((uint8_t*)0xdeadbeef, (uint8_t*)0xdeadbeef);

    // Called for index 0, 4
    EXPECT_TRUE(d.set_symbol_decoded.expect_calls()
                    .with(symbol_index{0U})
                    .with(symbol_index{4U})
                    .to_bool());
}
