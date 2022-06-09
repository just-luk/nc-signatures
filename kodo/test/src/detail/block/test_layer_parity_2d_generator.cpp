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

#include <kodo/detail/block/layer_parity_2d_generator.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    struct config
    {
    };
    void configure(const config&)
    {
    }

    stub::function<void(uint8_t*, byte_count)> zero_data;
    stub::function<void(uint8_t*, symbol_index, uint32_t)> set_value;
    stub::function<byte_count(symbol_count)> elements_to_bytes;
};
using stack = block::layer_parity_2d_generator<dummy>;
}

TEST(detail_block_test_layer_parity_2d_generator, configure)
{
    stack s;

    s.elements_to_bytes.set_return(byte_count{1});

    stack::config c;
    c.columns = symbol_count{5};
    c.rows = symbol_count{5};

    s.configure(c);
    EXPECT_EQ(c.rows, s.rows());
    EXPECT_EQ(c.columns, s.columns());
    EXPECT_NE(0U, s.max_position().value);
    EXPECT_NE(0U, s.symbols().value);
    EXPECT_EQ(1U, s.max_coefficients_bytes().value);
}
void test_parity_type_at(std::size_t rows, std::size_t columns,
                         std::vector<stack::parity_type> expectations)
{
    stack s;

    s.elements_to_bytes.set_return(byte_count{1});

    stack::config c;
    c.columns = symbol_count{columns};
    c.rows = symbol_count{rows};

    s.configure(c);

    ASSERT_EQ(expectations.size(), s.max_position().value + 1);
    auto row_repair = 0U;
    auto col_repair = 0U;
    for (std::size_t i = 0; i <= s.max_position().value; i++)
    {
        auto position = generator_position{i};
        stack::parity_type type;
        std::size_t index;
        std::tie(type, index) = s.parity_type_at(position);
        auto type_str = "";
        switch (type)
        {
        case stack::parity_type::source:
            type_str = "source";
            EXPECT_EQ(0U, index);
            EXPECT_FALSE(s.can_generate_at(position));
            break;
        case stack::parity_type::row_repair:
            type_str = "row_repair";
            EXPECT_EQ(row_repair, index);
            row_repair += 1;
            EXPECT_TRUE(s.can_generate_at(position));
            break;
        case stack::parity_type::column_repair:
            type_str = "col_repair";
            EXPECT_EQ(col_repair, index);
            col_repair += 1;
            EXPECT_TRUE(s.can_generate_at(position));
            break;
        default:
            EXPECT_FALSE(true);
        }

        EXPECT_EQ(expectations.at(i), type)
            << "position: " << i << " type " << type_str;
    }
    EXPECT_EQ(rows, row_repair);
    EXPECT_EQ(columns, col_repair);
}

TEST(detail_block_test_layer_parity_2d_generator, parity_type_at)
{
    using t = stack::parity_type;
    // clang-format off
    test_parity_type_at(1, 1, {
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(2, 1, {
        t::source, t::row_repair,

        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(1, 2, {
        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(2, 2, {
        t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(3, 4, {
        t::source, t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(4, 3, {
        t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(5, 5, {
        t::source, t::source, t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::source, t::source, t::row_repair,
        t::source, t::source, t::source, t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(2, 5, {
        t::source, t::source, t::source, t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});

    test_parity_type_at(5, 2, {
        t::source, t::source, t::row_repair,
        t::source, t::source, t::row_repair,
        t::source, t::source, t::row_repair,
        t::source, t::source, t::row_repair,

        t::source, t::column_repair,
        t::source, t::column_repair, t::row_repair});
    // clang-format on
}

TEST(detail_block_test_layer_parity_2d_generator, generate)
{
    stack s;

    s.elements_to_bytes.set_return(byte_count{1});

    stack::config c;
    c.columns = symbol_count{4};
    c.rows = symbol_count{3};

    s.configure(c);

    uint8_t* coefficients = (uint8_t*)0xdeadbeef;

    for (std::size_t i = 0; i <= s.max_position().value; i++)
    {
        auto position = generator_position{i};
        if (s.can_generate_at(position))
        {
            s.generate_specific(coefficients, position);
        }
    }

    // Make sure data is zeroed.
    EXPECT_TRUE(s.zero_data.expect_calls()
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .with(coefficients, s.max_coefficients_bytes())
                    .to_bool());

    EXPECT_TRUE(s.set_value
                    .expect_calls()
                    // row repair 0
                    .with(coefficients, symbol_index{0}, 1U)
                    .with(coefficients, symbol_index{1}, 1U)
                    .with(coefficients, symbol_index{2}, 1U)
                    .with(coefficients, symbol_index{3}, 1U)

                    // row repair 1
                    .with(coefficients, symbol_index{4}, 1U)
                    .with(coefficients, symbol_index{5}, 1U)
                    .with(coefficients, symbol_index{6}, 1U)
                    .with(coefficients, symbol_index{7}, 1U)

                    // col repair 0
                    .with(coefficients, symbol_index{0}, 1U)
                    .with(coefficients, symbol_index{4}, 1U)
                    .with(coefficients, symbol_index{8}, 1U)

                    // col repair 1
                    .with(coefficients, symbol_index{1}, 1u)
                    .with(coefficients, symbol_index{5}, 1u)
                    .with(coefficients, symbol_index{9}, 1u)

                    // col repair 2
                    .with(coefficients, symbol_index{2}, 1U)
                    .with(coefficients, symbol_index{6}, 1U)
                    .with(coefficients, symbol_index{10}, 1U)

                    // col repair 3
                    .with(coefficients, symbol_index{3}, 1U)
                    .with(coefficients, symbol_index{7}, 1U)
                    .with(coefficients, symbol_index{11}, 1U)

                    // row repair 2
                    .with(coefficients, symbol_index{8}, 1U)
                    .with(coefficients, symbol_index{9}, 1U)
                    .with(coefficients, symbol_index{10}, 1U)
                    .with(coefficients, symbol_index{11}, 1U)

                    .to_bool());
}
