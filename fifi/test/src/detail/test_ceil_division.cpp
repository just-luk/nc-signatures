// Copyright Steinwurf ApS 2012.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/detail/ceil_division.hpp>

#include <gtest/gtest.h>

TEST(test_ceil_division, ceil_division)
{
    EXPECT_EQ(fifi::detail::ceil_division(0, 1), 0U);
    EXPECT_EQ(fifi::detail::ceil_division(1, 1), 1U);
    EXPECT_EQ(fifi::detail::ceil_division(1, 2), 1U);
    EXPECT_EQ(fifi::detail::ceil_division(2, 1), 2U);
    EXPECT_EQ(fifi::detail::ceil_division(10, 3), 4U);
    EXPECT_EQ(fifi::detail::ceil_division(3, 10), 1U);
}
