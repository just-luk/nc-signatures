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

#include <kodo/slide/stream.hpp>

#include <gtest/gtest.h>

TEST(slide_test_stream, api)
{
    kodo::slide::stream<bool> stream;

    EXPECT_TRUE(stream.is_empty());
    EXPECT_EQ(0U, stream.size());
    EXPECT_EQ(0U, stream.lower_bound());
    EXPECT_EQ(0U, stream.upper_bound());

    std::size_t index = stream.push(true);
    EXPECT_EQ(0U, index);

    EXPECT_EQ(1U, stream.size());
    EXPECT_FALSE(stream.is_empty());
    EXPECT_EQ(0U, stream.lower_bound());
    EXPECT_EQ(1U, stream.upper_bound());
    EXPECT_TRUE(stream.in_stream(0U));

    index = stream.push(true);
    EXPECT_EQ(1U, index);

    EXPECT_EQ(2U, stream.size());
    EXPECT_FALSE(stream.is_empty());
    EXPECT_EQ(0U, stream.lower_bound());
    EXPECT_EQ(2U, stream.upper_bound());
    EXPECT_TRUE(stream.in_stream(0U));
    EXPECT_TRUE(stream.in_stream(1U));

    index = stream.push(false);
    EXPECT_EQ(2U, index);

    EXPECT_EQ(3U, stream.size());
    EXPECT_FALSE(stream.is_empty());
    EXPECT_EQ(0U, stream.lower_bound());
    EXPECT_EQ(3U, stream.upper_bound());
    EXPECT_TRUE(stream.in_stream(0U));
    EXPECT_TRUE(stream.in_stream(1U));
    EXPECT_TRUE(stream.in_stream(2U));

    index = stream.push(false);
    EXPECT_EQ(3U, index);

    EXPECT_EQ(4U, stream.size());
    EXPECT_EQ(0U, stream.lower_bound());
    EXPECT_EQ(4U, stream.upper_bound());
    EXPECT_TRUE(stream.in_stream(0U));
    EXPECT_TRUE(stream.in_stream(1U));
    EXPECT_TRUE(stream.in_stream(2U));
    EXPECT_TRUE(stream.in_stream(3U));

    index = stream.pop();
    EXPECT_EQ(0U, index);

    EXPECT_EQ(3U, stream.size());
    EXPECT_EQ(1U, stream.lower_bound());
    EXPECT_EQ(4U, stream.upper_bound());
    EXPECT_FALSE(stream.in_stream(0U));
    EXPECT_TRUE(stream.in_stream(1U));
    EXPECT_TRUE(stream.in_stream(2U));
    EXPECT_TRUE(stream.in_stream(3U));

    index = stream.push(true);
    EXPECT_EQ(4U, index);

    EXPECT_EQ(4U, stream.size());
    EXPECT_EQ(1U, stream.lower_bound());
    EXPECT_EQ(5U, stream.upper_bound());
    EXPECT_FALSE(stream.in_stream(0U));
    EXPECT_TRUE(stream.in_stream(1U));
    EXPECT_TRUE(stream.in_stream(2U));
    EXPECT_TRUE(stream.in_stream(3U));
    EXPECT_TRUE(stream.in_stream(4U));

    index = stream.pop();
    EXPECT_EQ(1U, index);

    EXPECT_EQ(3U, stream.size());
    EXPECT_EQ(2U, stream.lower_bound());
    EXPECT_EQ(5U, stream.upper_bound());
    EXPECT_FALSE(stream.in_stream(0U));
    EXPECT_FALSE(stream.in_stream(1U));
    EXPECT_TRUE(stream.in_stream(2U));
    EXPECT_TRUE(stream.in_stream(3U));
    EXPECT_TRUE(stream.in_stream(4U));
    EXPECT_FALSE(stream.in_stream(5U));

    index = stream.pop();
    EXPECT_EQ(2U, stream.size());

    index = stream.pop();
    EXPECT_EQ(1U, stream.size());

    index = stream.pop();
    EXPECT_EQ(0U, stream.size());
    EXPECT_TRUE(stream.is_empty());
}
