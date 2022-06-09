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

#include <kodo/block/generator/parity_2d.hpp>

#include <kodo/to_string.hpp>

#include "../../helper/combine.hpp"
#include "../../helper/get_value.hpp"
#include "../../helper/random_symbols.hpp"

#include <gtest/gtest.h>

#include <cstring>

using namespace kodo::block;

TEST(block_generator_test_parity_2d, symbols)
{
    generator::parity_2d generator;

    std::size_t rows = 8;
    std::size_t columns = 10;

    generator.configure(rows, columns);

    EXPECT_EQ(columns * rows, generator.symbols());
    EXPECT_EQ(10U, generator.max_coefficients_bytes());
}

TEST(block_generator_test_parity_2d, modes)
{
    generator::parity_2d generator;

    std::size_t rows = 8;
    std::size_t columns = 10;

    generator.configure(rows, columns);
    EXPECT_TRUE(generator.row_redundancy_enabled());
    EXPECT_TRUE(generator.column_redundancy_enabled());
    generator.set_row_redundancy_enabled(false);
    EXPECT_FALSE(generator.row_redundancy_enabled());
    EXPECT_TRUE(generator.column_redundancy_enabled());
    generator.set_column_redundancy_enabled(false);
    EXPECT_FALSE(generator.row_redundancy_enabled());
    EXPECT_FALSE(generator.column_redundancy_enabled());
    generator.set_column_redundancy_enabled(true);
    generator.set_row_redundancy_enabled(true);
    EXPECT_TRUE(generator.row_redundancy_enabled());
    EXPECT_TRUE(generator.column_redundancy_enabled());
}

namespace
{
void test_generate(std::size_t rows, std::size_t columns,
                   const std::vector<bool>& expected_matrix)
{
    SCOPED_TRACE(::testing::Message()
                 << "rows: " << rows << " columns: " << columns);
    generator::parity_2d generator;
    generator.configure(rows, columns);

    ASSERT_NE(0U, generator.max_coefficients_bytes());

    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::vector<bool> can_generates;
    // Generate the coefficients
    while (generator.can_advance())
    {
        can_generates.push_back(generator.can_generate());
        if (generator.can_generate())
            generator.generate(coefficients.data());
        generator.advance();
    }
    EXPECT_EQ(expected_matrix.size(), can_generates.size());
    EXPECT_EQ(expected_matrix, can_generates);
}
}

TEST(block_generator_test_parity_2d, generate)
{
    // clang-format off
    std::vector<std::tuple<std::size_t, std::size_t, std::vector<bool>>>
        setups = {
            std::make_tuple(3, 3, std::vector<bool>({
                false, false, false, true,
                false, false, false, true,
                false, true, false, true, false, true, true,
            })), std::make_tuple(5, 3, std::vector<bool>({
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
                false, true, false, true, false, true, true,
            })), std::make_tuple(3, 5, std::vector<bool>({
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, true, false, true, false, true, false, true, false, true, true
            })), std::make_tuple(5, 5, std::vector<bool>({
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, true, false, true, false, true, false, true, false, true, true
            }))
    };
    // clang-format on

    for (auto setup : setups)
    {
        auto rows = std::get<0>(setup);
        auto columns = std::get<1>(setup);
        auto expected_matrix = std::get<2>(setup);
        test_generate(rows, columns, expected_matrix);
    }
}

namespace
{
void test_generate_1d(std::size_t rows, std::size_t columns, bool enabled_rows,
                      bool enabled_columns,
                      const std::vector<bool>& expected_matrix)
{
    SCOPED_TRACE(::testing::Message()
                 << "rows: " << rows << " columns: " << columns);
    generator::parity_2d generator;
    generator.configure(rows, columns);
    generator.set_column_redundancy_enabled(enabled_columns);
    generator.set_row_redundancy_enabled(enabled_rows);
    ASSERT_NE(0U, generator.max_coefficients_bytes());

    // Buffer to store the coefficients
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::vector<bool> can_generates;
    // Generate the coefficients
    while (generator.can_advance())
    {
        can_generates.push_back(generator.can_generate());
        if (generator.can_generate())
            generator.generate(coefficients.data());
        generator.advance();
    }
    EXPECT_EQ(expected_matrix.size(), can_generates.size());
    EXPECT_EQ(expected_matrix, can_generates);
}
}

TEST(block_generator_test_parity_2d, generate_1d)
{
    // clang-format off
    std::vector<std::tuple<std::size_t, std::size_t, bool, bool, std::vector<bool>>>
        setups = {
            std::make_tuple(3, 3, true, false, std::vector<bool>({
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
            })), std::make_tuple(5, 3, true, false, std::vector<bool>({
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
                false, false, false, true,
            })), std::make_tuple(3, 5, true, false, std::vector<bool>({
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
            })), std::make_tuple(5, 5, true, false, std::vector<bool>({
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
                false, false, false, false, false, true,
            })),
            std::make_tuple(3, 3, false, true, std::vector<bool>({
                false, false, false,
                false, false, false,
                false, true, false, true, false, true,
            })), std::make_tuple(5, 3, false, true, std::vector<bool>({
                false, false, false,
                false, false, false,
                false, false, false,
                false, false, false,
                false, true, false, true, false, true,
            })), std::make_tuple(3, 5, false, true, std::vector<bool>({
                false, false, false, false, false,
                false, false, false, false, false,
                false, true, false, true, false, true, false, true, false, true,
            })), std::make_tuple(5, 5, false, true, std::vector<bool>({
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, false, false, false, false,
                false, true, false, true, false, true, false, true, false, true,
            }))
    };
    // clang-format on

    for (auto setup : setups)
    {
        auto rows = std::get<0>(setup);
        auto columns = std::get<1>(setup);
        bool enabled_rows = std::get<2>(setup);
        bool enabled_columns = std::get<3>(setup);
        auto expected_matrix = std::get<4>(setup);
        test_generate_1d(rows, columns, enabled_rows, enabled_columns,
                         expected_matrix);
    }
}
