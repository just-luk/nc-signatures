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

#include <fifi/online/divide.hpp>

#include <gtest/gtest.h>

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
const expected_result<fifi::field::binary> results[] =
    {
        // m_input1, m_input2, m_result
        { 0, 1, 0 },
        { 1, 1, 1 },
    };

const expected_result<fifi::field::binary4> results4[] =
    {
        // m_input1, m_input2, m_result
        {  0,  4,  0 },
        {  0,  7,  0 },
        {  0,  8,  0 },
        {  0,  9,  0 },
        {  0,  9,  0 },
        {  0, 13,  0 },
        {  1,  1,  1 },
        {  1, 12, 10 },
        {  2,  9,  4 },
        {  2, 14,  6 },
        {  4,  8,  9 },
        {  6,  3,  2 },
        {  6,  4,  8 },
        {  9,  1,  9 },
        {  9, 13,  2 },
        { 10, 10,  1 },
        { 11,  6,  4 },
        { 11,  8,  3 },
        { 11,  8,  3 },
        { 11, 12,  2 },
        { 11, 14, 14 },
        { 12,  2,  6 },
        { 12,  8,  8 },
        { 14,  5,  8 },
        { 14, 14,  1 },
    };

const expected_result<fifi::field::binary8> results8[] =
    {
        // arg1, arg2, result
        {   0,   2,   0 },
        {   1,   1,   1 },
        {   1,   2, 142 },
        {   1,  46,  51 },
        {   2,   1,   2 },
        {   4, 200, 111 },
        {   5,   5,   1 },
        {  10,  10,   1 },
        {  10,  20, 142 },
        {  12,  87, 182 },
        {  25,  10,  42 },
        {  25,  15, 199 },
        {  25,  66,  80 },
        {  30, 242, 199 },
        {  67,   1,  67 },
        { 110,   7, 125 },
        { 110, 253,  52 },
        { 144,  66, 254 },
        { 176, 132,  68 },
        { 200, 200,   1 },
        { 251, 133,  52 },
        { 254, 254,   1 },
        { 255, 255,   1 },
    };

const expected_result<fifi::field::binary16> results16[] =
    {
        // arg1, arg2, result
        {   0,   2,     0 },
        {   1,   1,     1 },
        {   1,   2, 34821 },
        {   1,  46,  2983 },
        {   2,   1,     2 },
        {   4, 200, 28050 },
        {   5,   5,     1 },
        {  10,  10,     1 },
        {  10,  20, 34821 },
        {  12,  87, 46287 },
        {  25,  10, 55300 },
        {  25,  15, 24577 },
        {  25,  66, 55164 },
        {  30, 242, 34597 },
        {  67,   1,    67 },
        { 110,   7, 54984 },
        { 110, 253,   825 },
        { 144,  66, 54045 },
        { 176, 132,  2247 },
        { 200, 200,     1 },
        { 251, 133, 46436 },
        { 254, 254,     1 },
        { 255, 255,     1 },
    };

const expected_result<fifi::field::prime2325> results2325[] =
    {
        // arg1,       arg2,        result
        {          1U, 4294967290U, 4294967290U },
        {          2U, 4294967290U, 4294967289U },
        {  163139403U, 1828949500U, 2835205604U },
        {  623898484U,   59902254U, 1708820736U },
        { 1078074232U, 1671507801U, 2817715425U },
        { 1190843567U, 1126933466U, 3113318947U },
        { 1336522198U, 1280630940U,   91488717U },
        { 1380709308U, 3151385314U, 4242837144U },
        { 2019008287U, 1889765579U, 2374943388U },
        { 2271514114U,  542032264U, 2085796361U },
        { 2622046040U,  209860049U, 1117407655U },
        { 2639979264U, 1669621279U, 3855275193U },
        { 3170796863U, 1056595135U, 2349732681U },
        { 3235144123U,  393597417U, 3381961331U },
        { 3846022732U, 2429951133U, 2542996908U },
        { 4026952922U,  589957415U, 1126790683U },
        { 4031746231U, 1150048672U, 2067171649U },
        { 4161263482U, 1084683978U,   57951883U },
        { 4188684512U,  871209300U, 4103154428U },
        { 4294967290U,          1U, 4294967290U },
        { 4294967290U,          2U, 2147483645U },
        { 4294967290U, 4294967290U,          1U },
    };

// clang-format on

}

TEST(test_divide, binary)
{
    constexpr fifi::field::binary field;

    for (auto result : results)
    {
        uint32_t actual =
            fifi::online::divide(field, result.m_input1, result.m_input2);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_divide, binary4)
{
    constexpr fifi::field::binary4 field(0b10011);

    for (auto result : results4)
    {
        uint32_t actual =
            fifi::online::divide(field, result.m_input1, result.m_input2);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_divide, binary8)
{
    constexpr fifi::field::binary8 field(0x11D);

    for (auto result : results8)
    {
        uint32_t actual =
            fifi::online::divide(field, result.m_input1, result.m_input2);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_divide, binary16)
{
    constexpr fifi::field::binary16 field(0x1100B);

    for (auto result : results16)
    {
        uint16_t actual =
            fifi::online::divide(field, result.m_input1, result.m_input2);

        uint16_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_divide, prime2325)
{
    constexpr fifi::field::prime2325 field;

    for (auto result : results2325)
    {
        uint32_t actual =
            fifi::online::divide(field, result.m_input1, result.m_input2);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}
