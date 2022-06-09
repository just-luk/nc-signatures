// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <fifi/detail/enabled_ptr.hpp>

#include <gtest/gtest.h>

namespace
{
struct dummy
{
    bool hello() const
    {
        return true;
    }

    bool is_compiled() const
    {
        return true;
    }
};
}

TEST(test_enabled_ptr, construct)
{
    dummy d;

    fifi::detail::enabled_ptr<dummy> p1{&d, false};
    EXPECT_FALSE(p1.is_enabled());

    fifi::detail::enabled_ptr<dummy> p2{&d, true};
    EXPECT_TRUE(p2.is_enabled());
    EXPECT_TRUE(p2->hello());
}
