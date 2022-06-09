// Copyright Steinwurf ApS 2019.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/to_string.hpp>

#include <gtest/gtest.h>

TEST(test_to_string, to_string)
{
    {
        SCOPED_TRACE("binary");
        auto field = fifi::finite_field::binary;
        EXPECT_EQ("binary", fifi::to_string(field));
    }

    {
        SCOPED_TRACE("binary4");
        auto field = fifi::finite_field::binary4;
        EXPECT_EQ("binary4", fifi::to_string(field));
    }

    {
        SCOPED_TRACE("binary8");
        auto field = fifi::finite_field::binary8;
        EXPECT_EQ("binary8", fifi::to_string(field));
    }

    {
        SCOPED_TRACE("binary16");
        auto field = fifi::finite_field::binary16;
        EXPECT_EQ("binary16", fifi::to_string(field));
    }

    {
        SCOPED_TRACE("prime2325");
        auto field = fifi::finite_field::prime2325;
        EXPECT_EQ("prime2325", fifi::to_string(field));
    }
}