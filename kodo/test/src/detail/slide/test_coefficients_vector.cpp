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

#include <kodo/detail/slide/coefficients_vector.hpp>

#include <cstring>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_slide_test_coefficients_vector, api)
{
    auto pool = slide::coefficients_vector::memory_pool();
    auto data_a = pool.allocate();
    data_a->resize(100);
    memset(data_a->data(), 'x', data_a->size());

    // Create a vector containing data_a
    slide::coefficients_vector v(byte_index{21}, std::move(data_a));

    EXPECT_EQ(byte_index{21U}, v.lower_bound());
    EXPECT_EQ(byte_count{100U}, v.bytes());
    EXPECT_EQ(byte_index{121U}, v.upper_bound());

    // Extend the data vector
    v.extend(byte_index{150});

    EXPECT_EQ(byte_index{21U}, v.lower_bound());
    EXPECT_EQ(byte_count{129U}, v.bytes());
    EXPECT_EQ(byte_index{150U}, v.upper_bound());
}

TEST(detail_slide_test_coefficients_vector, set)
{
    auto pool = slide::coefficients_vector::memory_pool();
    auto data_a = pool.allocate();
    data_a->resize(100);
    memset(data_a->data(), 'x', data_a->size());

    auto data_b = pool.allocate();
    data_b->resize(10);
    memset(data_b->data(), 'y', data_b->size());

    auto data_c = pool.allocate();
    data_c->resize(100);
    memset(data_c->data(), 'x', data_c->size());

    // Create a vector containing data_a
    slide::coefficients_vector v(byte_index{10}, std::move(data_a));

    slide::const_coefficients_view the_view(data_b->data(), byte_index{20},
                                            byte_count{10});

    v.set(the_view);

    // Create a vector to match
    std::vector<uint8_t> check;
    check.resize(10, 'x');
    check.resize(20, 'y');
    check.resize(100, 'x');
    EXPECT_EQ(0, std::memcmp(v.data(), check.data(), v.bytes().value));
}
