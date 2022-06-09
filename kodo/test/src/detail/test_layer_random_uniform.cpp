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

#include <kodo/detail/layer_random_uniform.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

namespace
{
struct dummy_layer
{
};

using stack = kodo::detail::layer_random_uniform<dummy_layer>;
}

TEST(detail_test_layer_random_uniform, generate)
{
    srand(time(0));
    stack s;

    s.set_seed(43);

    kodo::detail::byte_count bytes = kodo::detail::byte_count{100};

    std::vector<uint8_t> data(bytes.value);
    std::vector<uint8_t> empty_data(bytes.value);

    EXPECT_EQ(empty_data, data);
    s.generate_data(data.data(), bytes);

    EXPECT_NE(empty_data, data);
}
