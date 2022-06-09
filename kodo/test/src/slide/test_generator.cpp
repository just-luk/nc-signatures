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

#include <kodo/slide/generator/random_uniform.hpp>

#include <numeric>

#include <gtest/gtest.h>

TEST(slide_generator_test_random_uniform, field)
{
    kodo::slide::generator::random_uniform generator;

    {
        generator =
            kodo::slide::generator::random_uniform(kodo::finite_field::binary);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary);
    }
    {
        generator =
            kodo::slide::generator::random_uniform(kodo::finite_field::binary4);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary4);
    }
    {
        generator =
            kodo::slide::generator::random_uniform(kodo::finite_field::binary8);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary8);
    }
    {
        generator = kodo::slide::generator::random_uniform(
            kodo::finite_field::binary16);
        EXPECT_EQ(generator.field(), kodo::finite_field::binary16);
    }
}

TEST(slide_generator_test_random_uniform, basic_api)
{
    kodo::finite_field field = kodo::finite_field::binary8;

    kodo::slide::generator::random_uniform generator(field);

    kodo::slide::range window{4, 12};
    EXPECT_EQ(8U, generator.coefficients_bytes(window));

    std::vector<uint8_t> coefficients(generator.coefficients_bytes(window));

    generator.set_seed(rand());
    generator.generate(coefficients.data(), window);

    // The coefficient vector must not be all zero. To check this we simply
    // sum the values in the vector and see that it is not equal to 0.
    uint32_t coefficients_sum =
        std::accumulate(coefficients.begin(), coefficients.end(), 0U);

    EXPECT_NE(0U, coefficients_sum);
}
