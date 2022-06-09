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

#include <kodo/block/generator/rs_cauchy.hpp>

#include <kodo/to_string.hpp>

#include "../../helper/combine.hpp"
#include "../../helper/get_value.hpp"
#include "../../helper/random_symbols.hpp"

#include <gtest/gtest.h>

#include <cstring>

using namespace kodo::block;

TEST(block_generator_test_rs_cauchy, field)
{
    {
        SCOPED_TRACE(::testing::Message() << "Field: binary4");
        generator::rs_cauchy generator(kodo::finite_field::binary4);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary4);
    }
    {
        SCOPED_TRACE(::testing::Message() << "Field: binary8");
        generator::rs_cauchy generator(kodo::finite_field::binary8);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary8);
    }
}
TEST(block_generator_test_rs_cauchy, symbols)
{
    generator::rs_cauchy generator(kodo::finite_field::binary8);

    std::size_t symbols = 100;

    generator.configure(symbols);

    EXPECT_EQ(155U, generator.repair_symbols());
    EXPECT_EQ(155U, generator.remaining_repair_symbols());
    EXPECT_EQ(symbols, generator.symbols());
    EXPECT_EQ(100U, generator.max_coefficients_bytes());

    symbols = 10;
    generator.configure(symbols);

    EXPECT_EQ(245U, generator.repair_symbols());
    EXPECT_EQ(245U, generator.remaining_repair_symbols());
    EXPECT_EQ(10U, generator.max_coefficients_bytes());
    EXPECT_EQ(symbols, generator.symbols());
}

namespace
{
void test_generate(std::size_t symbols)
{
    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    generator::rs_cauchy generator(kodo::finite_field::binary8);
    generator.configure(symbols);

    ASSERT_NE(0U, generator.max_coefficients_bytes());
    ASSERT_NE(0U, generator.repair_symbols());
    ASSERT_NE(0U, generator.remaining_repair_symbols());
    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Generate the coefficients
    generator.generate(coefficients.data());
    EXPECT_EQ(generator.repair_symbols() - 1,
              generator.remaining_repair_symbols());

    bool all_zeros = true;
    for (auto data : coefficients)
    {
        if (data != 0)
            all_zeros = false;
    }

    EXPECT_FALSE(all_zeros);

    generator.reset();

    EXPECT_EQ(generator.repair_symbols(), generator.remaining_repair_symbols());

    std::vector<uint8_t> expected_coefficients(
        generator.max_coefficients_bytes());
    generator.generate(expected_coefficients.data());
    generator.generate_specific(coefficients.data(), 0);
    EXPECT_EQ(coefficients, expected_coefficients);
}
}

TEST(block_generator_test_rs_cauchy, generate)
{
    std::vector<std::size_t> symbols_counts{
        1, 10, 100, 200, 254, helper::random_symbols(1, 254)};
    for (auto symbols_count : symbols_counts)
    {
        test_generate(symbols_count);
    }
}
