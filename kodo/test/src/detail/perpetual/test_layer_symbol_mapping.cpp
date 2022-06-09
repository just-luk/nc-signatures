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

#include <kodo/detail/perpetual/layer_symbol_mapping.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;
using namespace kodo::detail::perpetual;

namespace
{
struct dummy
{
    stub::function<symbol_count()> symbols;
    stub::function<symbol_count()> zero_symbols;
    stub::function<symbol_count()> outer_interval;
};
using stack = layer_symbol_mapping<dummy>;
}

TEST(detail_perpetual_test_layer_symbol_mapping, mapping)
{

    stack s;
    s.symbols.set_return(symbol_count{100});
    s.zero_symbols.set_return(symbol_count{3});
    s.outer_interval.set_return(symbol_count{3});

    // Layout of symbols:
    //
    //    z0 z1 z3 r0 s1 s2 s3 r1 s4 s5 s6 r2 s7 ....
    //

    // Test the mapping to type

    EXPECT_EQ(s.map_to_type(symbol_index{0}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{1}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{2}), symbol_type::zero);

    EXPECT_EQ(s.map_to_type(symbol_index{3}), symbol_type::outer);

    EXPECT_EQ(s.map_to_type(symbol_index{4}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{5}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{6}), symbol_type::data);

    EXPECT_EQ(s.map_to_type(symbol_index{7}), symbol_type::outer);

    EXPECT_EQ(s.map_to_type(symbol_index{8}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{9}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{10}), symbol_type::data);

    EXPECT_EQ(s.map_to_type(symbol_index{11}), symbol_type::outer);

    EXPECT_EQ(s.map_to_type(symbol_index{12}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{13}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{14}), symbol_type::data);

    // Test the relative outer index

    EXPECT_EQ(s.to_outer_index(symbol_index{3}), outer_index{0});
    EXPECT_EQ(s.to_outer_index(symbol_index{7}), outer_index{1});
    EXPECT_EQ(s.to_outer_index(symbol_index{11}), outer_index{2});
}

TEST(detail_perpetual_test_layer_symbol_mapping, mapping_zero_outer)
{

    stack s;
    s.symbols.set_return(symbol_count{100});
    s.zero_symbols.set_return(symbol_count{3});
    s.outer_interval.set_return(symbol_count{0});

    // Layout of symbols:
    //
    //    z0 z1 z3 s1 s2 s3 s4 s5 s6 s7 ....
    //

    // Test the mapping to type

    EXPECT_EQ(s.map_to_type(symbol_index{0}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{1}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{2}), symbol_type::zero);

    EXPECT_EQ(s.map_to_type(symbol_index{3}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{4}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{5}), symbol_type::data);

    EXPECT_EQ(s.map_to_type(symbol_index{6}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{7}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{8}), symbol_type::data);

    EXPECT_EQ(s.map_to_type(symbol_index{9}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{10}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{11}), symbol_type::data);
}

TEST(detail_perpetual_test_layer_symbol_mapping, equal_symbols_and_outer)
{

    stack s;
    s.symbols.set_return(symbol_count{7});
    s.zero_symbols.set_return(symbol_count{3});
    s.outer_interval.set_return(symbol_count{3});

    // Layout of symbols:
    //
    //    z0 z1 z3 r0 s0 s1 s2
    //

    // Test the mapping to type

    EXPECT_EQ(s.map_to_type(symbol_index{0}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{1}), symbol_type::zero);
    EXPECT_EQ(s.map_to_type(symbol_index{2}), symbol_type::zero);

    EXPECT_EQ(s.map_to_type(symbol_index{3}), symbol_type::outer);
    EXPECT_EQ(s.map_to_type(symbol_index{4}), symbol_type::data);
    EXPECT_EQ(s.map_to_type(symbol_index{5}), symbol_type::data);

    EXPECT_EQ(s.map_to_type(symbol_index{6}), symbol_type::data);

    // Test the relative outer index

    EXPECT_EQ(s.to_outer_index(symbol_index{3}), outer_index{0});
}

TEST(detail_perpetual_test_layer_symbol_mapping, last_symbol_is_outer_symbol)
{
    stack s;
    s.symbols.set_return(symbol_count{12});
    s.zero_symbols.set_return(symbol_count{1});
    s.outer_interval.set_return(symbol_count{4});

    // Layout of symbols:
    //
    // Index:    0  1  2  3  4  5  6  7  8  9 10 11
    // Mapping: z0 r0 s0 s1 s2 s3 r1 s4 s5 s6 s7 r1
    //

    EXPECT_EQ(s.map_to_type(symbol_index{11}), symbol_type::outer);
}

TEST(detail_perpetual_test_layer_symbol_mapping, test_mapping_out_of_bounds)
{
    stack s;
    s.symbols.set_return(symbol_count{15432});
    s.zero_symbols.set_return(symbol_count{0});
    s.outer_interval.set_return(symbol_count{4});

    // outer symbols = 12346 / 4 = 3086

    EXPECT_EQ(s.map_to_type(symbol_index{15430}), symbol_type::outer);
    EXPECT_EQ(s.to_outer_index(symbol_index{15430}), outer_index{3086});
}
