// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#include <fifi/field/prime2325_apply_prefix.hpp>

#include <algorithm>
#include <gtest/gtest.h>

#include <fifi/field/bytes_to_elements.hpp>
#include <fifi/field/get_value.hpp>

TEST(test_prime2325_apply_prefix, prime2325_apply_prefix)
{
    using namespace fifi::field;

    std::size_t size = 1000;

    std::vector<uint8_t> x(size);
    std::generate(x.begin(), x.end(), rand);
    std::vector<uint8_t> y = x;

    uint32_t prefix = rand() % prime2325().prime();

    SCOPED_TRACE(testing::Message() << "prefix: " << prefix);

    prime2325_apply_prefix(x.data(), x.size(), prefix);

    std::size_t elements = fifi::field::bytes_to_elements(prime2325(), size);

    for (std::size_t i = 0; i < elements; ++i)
    {
        uint32_t x_value = fifi::field::get_value(prime2325(), x.data(), i);
        uint32_t y_value = fifi::field::get_value(prime2325(), y.data(), i);

        EXPECT_EQ(y_value ^ prefix, x_value);
    }
}
