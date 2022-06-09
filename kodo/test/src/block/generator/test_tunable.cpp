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

#include <kodo/block/generator/tunable.hpp>

#include <kodo/to_string.hpp>

#include <gtest/gtest.h>

#include <cstring>

using namespace kodo::block;

TEST(block_generator_test_tunable, field)
{
    generator::tunable generator;

    {
        generator = generator::tunable(kodo::finite_field::binary);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary);
    }
    {
        generator = generator::tunable(kodo::finite_field::binary4);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary4);
    }
    {
        generator = generator::tunable(kodo::finite_field::binary8);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary8);
    }
    {
        generator = generator::tunable(kodo::finite_field::binary16);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary16);
    }
}

TEST(block_generator_test_tunable, seed)
{
    generator::tunable generator{kodo::finite_field::binary};

    std::size_t symbols = 100;

    generator.configure(symbols);
    generator.set_seed(42U);

    EXPECT_EQ(symbols, generator.symbols());
    EXPECT_EQ(13U, generator.max_coefficients_bytes());

    generator.configure(10U);
    generator.set_seed(46U);

    EXPECT_EQ(2U, generator.max_coefficients_bytes());
}

namespace
{
void test_generate(kodo::finite_field field, std::size_t symbols, float density)
{

    SCOPED_TRACE(::testing::Message()
                 << "finite_field: " << kodo::to_string(field));
    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    generator::tunable generator{field};
    generator.configure(symbols);

    ASSERT_NE(0U, generator.max_coefficients_bytes());
    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Generate the coefficients
    generator.generate(coefficients.data(), density);

    bool all_zeros = true;
    for (auto data : coefficients)
    {
        if (data != 0)
            all_zeros = false;
    }

    EXPECT_FALSE(all_zeros);
}
}

TEST(block_generator_test_tunable, generate)
{
    test_generate(kodo::finite_field::binary, 10, 0.01f);
    test_generate(kodo::finite_field::binary4, 10, 0.25f);
    test_generate(kodo::finite_field::binary8, 10, 0.50f);
    test_generate(kodo::finite_field::binary16, 10, 1.00f);
}

namespace
{
auto count_non_zeros(std::size_t symbols, float density, uint32_t seed)
    -> std::size_t
{
    generator::tunable generator{kodo::finite_field::binary8};
    generator.configure(symbols);
    generator.set_seed(seed);
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    generator.generate(coefficients.data(), density);

    std::size_t non_zeros = 0;
    for (auto data : coefficients)
    {
        if (data != 0)
            non_zeros += 1;
    }
    return non_zeros;
}
}

TEST(block_generator_test_tunable, test_non_zeros)
{
    EXPECT_EQ(1U, count_non_zeros(10, 0.01f, 0));
    EXPECT_EQ(2U, count_non_zeros(10, 0.2f, 0));
    EXPECT_EQ(10U, count_non_zeros(100, 0.1f, 1));
    EXPECT_EQ(100U, count_non_zeros(100, 1.0f, 1));
}

void test_seed(kodo::finite_field field, std::size_t symbols, float density,
               std::size_t seed)
{
    SCOPED_TRACE(::testing::Message()
                 << "finite_field: " << kodo::to_string(field));
    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    generator::tunable generator{field};
    generator.configure(symbols);
    generator.set_seed(seed);

    ASSERT_NE(0U, generator.max_coefficients_bytes());
    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients1(generator.max_coefficients_bytes());
    std::vector<uint8_t> coefficients2(generator.max_coefficients_bytes());

    // Generate the coefficients
    generator.generate(coefficients1.data(), density);
    generator.generate(coefficients2.data(), density);
    EXPECT_NE(coefficients1, coefficients2);

    std::vector<uint8_t> coefficients3(generator.max_coefficients_bytes());
    generator.set_seed(seed);
    generator.generate(coefficients3.data(), density);
    EXPECT_EQ(coefficients1, coefficients3);
}

TEST(block_generator_test_tunable, test_seed)
{
    test_seed(kodo::finite_field::binary, 10, 0.01f, 0);
    test_seed(kodo::finite_field::binary, 10, 0.2f, 0);
    test_seed(kodo::finite_field::binary, 100, 0.5f, 1);

    test_seed(kodo::finite_field::binary4, 10, 0.01f, 0);
    test_seed(kodo::finite_field::binary4, 10, 0.2f, 0);
    test_seed(kodo::finite_field::binary4, 100, 0.5f, 1);

    test_seed(kodo::finite_field::binary8, 10, 0.01f, 0);
    test_seed(kodo::finite_field::binary8, 10, 0.2f, 0);
    test_seed(kodo::finite_field::binary8, 100, 0.5f, 1);

    test_seed(kodo::finite_field::binary16, 10, 0.01f, 0);
    test_seed(kodo::finite_field::binary16, 10, 0.2f, 0);
    test_seed(kodo::finite_field::binary16, 100, 0.5f, 1);
}
