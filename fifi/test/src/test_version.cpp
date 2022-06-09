// Copyright Steinwurf ApS 2019.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/version.hpp>

#include <gtest/gtest.h>

TEST(test_version, not_empty)
{
    EXPECT_FALSE(fifi::version().empty());
}
