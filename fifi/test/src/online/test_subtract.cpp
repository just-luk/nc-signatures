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

#include <fifi/online/subtract.hpp>

#include <gtest/gtest.h>

#include "../helper/random_value.hpp"

namespace
{
template <class Field>
struct expected_result
{
    typename Field::value_type m_input1;
    typename Field::value_type m_input2;
    typename Field::value_type m_result;
};

// clang-format off
const expected_result<fifi::field::prime2325> results2325[] =
    {
        // arg1,       arg2,        result
        {          0U,          0U,          0U },
        {          1U, 4294967290U,          2U },
        {          2U, 4294967290U,          3U },
        {  815150013U, 2783628454U, 2326488850U },
        { 1111197069U, 3288497105U, 2117667255U },
        { 1376464298U, 3719246614U, 1952184975U },
        { 1555616877U, 417999038U,  1137617839U },
        { 1591755985U, 760357706U,   831398279U },
        { 1646681944U, 2090570642U, 3851078593U },
        { 1718415100U, 3764394937U, 2248987454U },
        { 2021566441U, 1563544409U,  458022032U },
        { 2279203252U, 2364542188U, 4209628355U },
        { 2313466124U, 1242826905U, 1070639219U },
        { 2486487802U, 4101049708U, 2680405385U },
        { 3007334859U, 2068575905U,  938758954U },
        { 3267843829U, 3990959519U, 3571851601U },
        { 3448691623U, 2270397823U, 1178293800U },
        { 3601154686U, 238248285U,  3362906401U },
        { 3850830140U, 4242429948U, 3903367483U },
        { 4181356062U, 2914712094U, 1266643968U },
        { 4294967290U,          1U, 4294967289U },
        { 4294967290U,          2U, 4294967288U },
        { 4294967290U, 4294967290U,          0U },
        { 1725414168U, 591653559U,  1133760609U },
    };

// clang-format on

template <class Field>
inline void test_binary_subtract(const Field& field)
{
    for (uint32_t i = 0; i < 100; ++i)
    {
        uint32_t a = helper::random_value(field);
        uint32_t b = helper::random_value(field);

        EXPECT_EQ(a ^ b, fifi::online::subtract(field, a, b));
    }
}
}

TEST(test_subtract, binary)
{
    test_binary_subtract(fifi::field::binary());
}

TEST(test_subtract, binary4)
{
    test_binary_subtract(fifi::field::binary4());
}

TEST(test_subtract, binary8)
{
    test_binary_subtract(fifi::field::binary8());
}

TEST(test_subtract, binary16)
{
    test_binary_subtract(fifi::field::binary16());
}

TEST(test_subtract, prime2325)
{
    constexpr fifi::field::prime2325 field;

    for (auto result : results2325)
    {
        uint32_t actual =
            fifi::online::subtract(field, result.m_input1, result.m_input2);

        EXPECT_EQ(result.m_result, actual);
    }
}
