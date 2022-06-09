// Copyright Steinwurf ApS 2019.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/to_field.hpp>

#include <gtest/gtest.h>

TEST(test_to_field, to_field)
{
    {
        SCOPED_TRACE("binary");
        auto value = "binary";
        EXPECT_EQ(fifi::finite_field::binary, fifi::to_field(value));
    }

    {
        SCOPED_TRACE("binary4");
        auto value = "binary4";
        EXPECT_EQ(fifi::finite_field::binary4, fifi::to_field(value));
    }

    {
        SCOPED_TRACE("binary8");
        auto value = "binary8";
        EXPECT_EQ(fifi::finite_field::binary8, fifi::to_field(value));
    }

    {
        SCOPED_TRACE("binary16");
        auto value = "binary16";
        EXPECT_EQ(fifi::finite_field::binary16, fifi::to_field(value));
    }

    {
        SCOPED_TRACE("prime2325");
        auto value = "prime2325";
        EXPECT_EQ(fifi::finite_field::prime2325, fifi::to_field(value));
    }
}