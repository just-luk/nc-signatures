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

#include <kodo/detail/last.hpp>

#include <kodo/detail/range.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_last, api)
{
    using range_type = range<std::size_t, struct _integer>;

    EXPECT_EQ(4U, last(range_type{1, 5}));
    EXPECT_EQ(5U, last(range_type{2, 6}));
    EXPECT_EQ(6U, last(range_type{3, 7}));
    EXPECT_EQ(7U, last(range_type{4, 8}));
}
