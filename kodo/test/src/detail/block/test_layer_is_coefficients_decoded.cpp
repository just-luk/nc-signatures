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

#include <kodo/detail/block/layer_coefficients_storage.hpp>
#include <kodo/detail/block/layer_is_coefficients_decoded.hpp>
#include <kodo/detail/byte_count.hpp>
#include <kodo/detail/byte_index.hpp>
#include <kodo/detail/layer_symbols.hpp>
#include <kodo/detail/symbol_count.hpp>
#include <kodo/detail/symbol_index.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

namespace
{
struct dummy_layer
{
    stub::function<const uint8_t*(kodo::detail::symbol_index)>
        coefficients_data;

    stub::function<kodo::detail::symbol_range()> symbol_range;

    stub::function<uint32_t(const uint8_t*, kodo::detail::symbol_index)>
        get_value;
};
}

TEST(detail_block_test_layer_is_coefficients_decoded, not_decoded)
{
    using namespace kodo::detail;

    block::layer_is_coefficients_decoded<dummy_layer> stack;

    dummy_layer& d = stack;

    d.coefficients_data.set_return((const uint8_t*)0xdeadbeef);

    d.symbol_range.set_return(symbol_range{symbol_index{10}, symbol_index{15}});

    // Will be called with index 10, 11, 13, 14
    d.get_value.set_return(0U, 0U, 1U);

    EXPECT_FALSE(stack.is_coefficients_decoded(symbol_index{12}));
}

TEST(detail_block_test_layer_is_coefficients_decoded, decoded)
{
    using namespace kodo::detail;

    block::layer_is_coefficients_decoded<dummy_layer> stack;

    dummy_layer& d = stack;

    d.coefficients_data.set_return((const uint8_t*)0xdeadbeef);

    d.symbol_range.set_return(symbol_range{symbol_index{10}, symbol_index{15}});

    // Will be called with index 10, 11, 13, 14
    d.get_value.set_return(0U, 0U, 0U, 0U);

    EXPECT_TRUE(stack.is_coefficients_decoded(symbol_index{12}));
}
