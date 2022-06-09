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

#include <kodo/detail/to_bitset.hpp>

#include <gtest/gtest.h>

using namespace kodo::detail;

TEST(detail_test_to_bitset, api)
{
    bitset bs{bit_count{10000000}};

    for (std::size_t i = 0; i < bs.bits().value; i++)
    {
        if (rand() % 2 == 0)
        {
            bs.set(bit_index{i});
        }
    }
    EXPECT_NE(bit_count{0U}, bs.count_ones());
    std::vector<uint8_t> buffer(bs.storage(),
                                bs.storage() + bs.storage_bytes().value);
    auto bs_deserialized = to_bitset(buffer.data(), bs.bits());

    bit_index index{rand() % bs.bits().value};
    EXPECT_EQ(bs.bits(), bs_deserialized.bits());
    EXPECT_EQ(bs.get(index), bs_deserialized.get(index));
    EXPECT_EQ(bs.count_ones(), bs_deserialized.count_ones());
    EXPECT_EQ(bs.lsb(), bs_deserialized.lsb());
    EXPECT_EQ(bs.msb(), bs_deserialized.msb());
    EXPECT_EQ(bs.lower_bound(), bs_deserialized.lower_bound());
    EXPECT_EQ(bs.upper_bound(), bs_deserialized.upper_bound());
}
