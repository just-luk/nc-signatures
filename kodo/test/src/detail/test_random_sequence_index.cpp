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

#include <kodo/detail/random_sequence_index.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

static void test_random_sequence_index(symbol_count symbols)
{
    SCOPED_TRACE(testing::Message() << "symbols: " << symbols);

    random_sequence_index generator{symbols};

    std::vector<uint32_t> hits(symbols.value, 0);

    for (uint32_t i = 0; i < symbols.value * 3; ++i)
    {
        symbol_index index = generator.index();
        hits[index.value] += 1U;
        generator.advance();
    }

    for (std::size_t hit : hits)
    {
        EXPECT_EQ(3U, hit);
    }
}

TEST(test_random_sequence_index, api)
{
    std::vector<symbol_count> symbols{symbol_count{1U}, symbol_count{2U},
                                      symbol_count{1000U}, symbol_count{3243U}};

    for (symbol_count s : symbols)
    {
        test_random_sequence_index(s);
    }
}
