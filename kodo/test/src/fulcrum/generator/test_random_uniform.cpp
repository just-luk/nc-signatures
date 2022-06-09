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

#include <kodo/fulcrum/generator/random_uniform.hpp>

#include <kodo/to_string.hpp>

#include "../../helper/combine.hpp"
#include "../../helper/get_value.hpp"
#include "../../helper/random_symbols.hpp"

#include <gtest/gtest.h>

#include <cstring>

using namespace kodo::fulcrum;

TEST(fulcrum_generator_test_random_uniform, field)
{
    generator::random_uniform generator;
    EXPECT_EQ(generator.field(), kodo::finite_field::binary);
}

TEST(fulcrum_generator_test_random_uniform, seed)
{
    generator::random_uniform generator;

    std::size_t symbols = 95;
    std::size_t expansion = 5;

    generator.configure(symbols, expansion);
    generator.set_seed(42U);

    EXPECT_EQ(symbols, generator.symbols());
    EXPECT_EQ(expansion, generator.expansion());
    EXPECT_EQ(13U, generator.max_coefficients_bytes());

    generator.configure(8U, 2U);
    generator.set_seed(46U);

    EXPECT_EQ(2U, generator.max_coefficients_bytes());
}

namespace
{
void test_generate(std::size_t symbols, std::size_t expansion)
{

    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    SCOPED_TRACE(::testing::Message() << "expansion: " << expansion);
    generator::random_uniform generator;
    generator.configure(symbols, expansion);

    ASSERT_NE(0U, generator.max_coefficients_bytes());
    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Generate the coefficients
    generator.generate(coefficients.data());

    bool all_zeros = true;
    for (auto data : coefficients)
    {
        if (data != 0)
            all_zeros = false;
    }

    EXPECT_FALSE(all_zeros);
}
}

TEST(fulcrum_generator_test_random_uniform, generate)
{

    std::vector<std::size_t> symbols_counts{1, 10, 100, 1000,
                                            helper::random_symbols()};
    std::vector<std::size_t> expansions{1, 10, 100, helper::random_symbols()};

    helper::combine(
        [](auto v)
        {
            std::size_t expansion = std::get<0>(v);
            std::size_t symbols_count = std::get<1>(v);
            test_generate(symbols_count, expansion);
        },
        expansions, symbols_counts);
}
