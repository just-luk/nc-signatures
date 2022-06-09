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

#include <kodo/perpetual/offset/random_uniform.hpp>

#include <gtest/gtest.h>

using namespace kodo::perpetual;

TEST(perpetual_offset_test_random_uniform, api)
{
    offset::random_uniform generator;

    std::size_t symbols = 100;

    generator.configure(symbols);
    generator.set_seed(rand());

    std::vector<std::size_t> hits(symbols, 0);

    // Since we have a width of 10 the highest offset that will be generated
    // is 90 since that offset will include symbols:
    //
    //  90 91 92 93 94 95 96 97 98 99
    //

    // Since the output is random however it is hard to test - what we will do
    // is to loop a bunch of times and ensure that all indicies from 0-90 is
    // hit at least once

    for (std::size_t i = 0; i < symbols * 25; ++i)
    {
        std::size_t offset = generator.offset();

        ASSERT_TRUE(offset < hits.size());

        hits[offset] += 1U;
    }

    for (std::size_t i = 0; i < hits.size(); ++i)
    {
        SCOPED_TRACE(testing::Message() << "i: " << i);
        EXPECT_TRUE(hits[i] > 0);
    }
}
