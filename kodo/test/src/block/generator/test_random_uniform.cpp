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

#include <kodo/block/generator/random_uniform.hpp>

#include <kodo/to_string.hpp>

#include "../../helper/combine.hpp"
#include "../../helper/get_value.hpp"
#include "../../helper/random_symbols.hpp"

#include <gtest/gtest.h>

#include <cstring>

using namespace kodo::block;

TEST(block_generator_test_random_uniform, field)
{
    generator::random_uniform generator;

    {
        generator = generator::random_uniform(kodo::finite_field::binary);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary);
    }
    {
        generator = generator::random_uniform(kodo::finite_field::binary4);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary4);
    }
    {
        generator = generator::random_uniform(kodo::finite_field::binary8);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary8);
    }
    {
        generator = generator::random_uniform(kodo::finite_field::binary16);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary16);
    }
}

TEST(block_generator_test_random_uniform, seed)
{
    generator::random_uniform generator{kodo::finite_field::binary};

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
void test_generate(kodo::finite_field field, std::size_t symbols)
{

    SCOPED_TRACE(::testing::Message()
                 << "finite_field: " << kodo::to_string(field));
    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    generator::random_uniform generator{field};
    generator.configure(symbols);

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

TEST(block_generator_test_random_uniform, generate)
{
    std::vector<kodo::finite_field> finite_fields{
        kodo::finite_field::binary,
        kodo::finite_field::binary4,
        kodo::finite_field::binary8,
        kodo::finite_field::binary16,
    };

    std::vector<std::size_t> symbols_counts{1, 10, 100, 1000,
                                            helper::random_symbols()};

    helper::combine(
        [](auto v)
        {
            kodo::finite_field field = std::get<0>(v);
            std::size_t symbols_count = std::get<1>(v);
            test_generate(field, symbols_count);
        },
        finite_fields, symbols_counts);
}

namespace
{
void test_generate_partial(kodo::finite_field field, std::size_t symbols)
{
    SCOPED_TRACE(::testing::Message()
                 << "finite_field: " << kodo::to_string(field));
    SCOPED_TRACE(::testing::Message() << "symbols: " << symbols);
    generator::random_uniform generator{field};
    generator.configure(symbols);
    generator.set_seed(0);

    ASSERT_NE(0U, generator.max_coefficients_bytes());
    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes(), 0);

    // Generate the coefficients
    generator.generate_partial(coefficients.data(), symbols - 1);

    bool all_zeros = true;
    for (auto data : coefficients)
    {
        if (data != 0)
            all_zeros = false;
    }
    EXPECT_NE(std::vector<uint8_t>(generator.max_coefficients_bytes(), 0),
              coefficients);
    EXPECT_FALSE(all_zeros);

    EXPECT_EQ(0U, helper::get_value(field, coefficients.data(), symbols - 1));

    /// Zero the coefficients.
    std::memset(coefficients.data(), 0, coefficients.size());

    // Generate the coefficients
    generator.generate_partial(coefficients.data(), 1);

    all_zeros = true;
    for (auto data : coefficients)
    {
        if (data != 0)
            all_zeros = false;
    }
    EXPECT_FALSE(all_zeros);
    EXPECT_NE(0U, helper::get_value(field, coefficients.data(), 0));
}
}

TEST(block_generator_test_random_uniform, generate_partial)
{
    std::vector<kodo::finite_field> finite_fields{
        kodo::finite_field::binary,
        kodo::finite_field::binary4,
        kodo::finite_field::binary8,
        kodo::finite_field::binary16,
    };

    std::vector<std::size_t> symbols_counts{2, 10, 100, 1000,
                                            helper::random_symbols(2)};

    helper::combine(
        [](auto v)
        {
            kodo::finite_field field = std::get<0>(v);
            std::size_t symbols_count = std::get<1>(v);
            test_generate_partial(field, symbols_count);
        },
        finite_fields, symbols_counts);
}

TEST(block_generator_test_random_uniform, more_generate_partial)
{
    auto field = kodo::finite_field::binary4;
    kodo::block::generator::random_uniform generator{field};
    generator.configure(2);
    ASSERT_NE(0U, generator.max_coefficients_bytes());

    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    // Generate the coefficients
    generator.generate_partial(coefficients.data(), 1);
    EXPECT_NE(0U, helper::get_value(field, coefficients.data(), 0));
    EXPECT_EQ(0U, helper::get_value(field, coefficients.data(), 1));
}
