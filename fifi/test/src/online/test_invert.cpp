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

#include <fifi/online/invert.hpp>

#include <gtest/gtest.h>

namespace
{
template <class Field>
struct expected_result
{
    typename Field::value_type m_input;
    typename Field::value_type m_result;
};

// clang-format off
const expected_result<fifi::field::binary> results[] =
    {
        // m_input1, m_result
        { 1, 1 },
    };

const expected_result<fifi::field::binary4> results4[] =
    {
        // m_input, m_result
        {  1,  1 },
        {  2,  9 },
        {  3, 14 },
        {  3, 14 },
        {  4, 13 },
        {  4, 13 },
        {  5, 11 },
        {  5, 11 },
        {  5, 11 },
        {  6,  7 },
        {  6,  7 },
        {  9,  2 },
        { 10, 12 },
        { 10, 12 },
        { 10, 12 },
        { 11,  5 },
        { 11,  5 },
        { 12, 10 },
        { 14,  3 },
        { 14,  3 },
        { 14,  3 },
        { 15,  8 },
        { 15,  8 },
        { 15,  8 },
        { 15,  8 },
    };

// Invert binary8
const expected_result<fifi::field::binary8> results8[] =
    {
        // arg1, result
        {   1,   1 },
        {   2, 142 },
        {   5, 167 },
        {  10, 221 },
        {  15, 150 },
        {  20, 224 },
        {  25, 222 },
        {  46,  51 },
        {  66, 248 },
        {  67, 213 },
        {  87,  97 },
        { 110,  47 },
        { 132, 124 },
        { 133, 204 },
        { 144,  24 },
        { 200, 210 },
        { 242, 181 },
        { 253, 255 },
        { 254, 126 },
        { 255, 253 },
    };

// Invert binary16
const expected_result<fifi::field::binary16> results16[] =
    {
        // arg1, result
        {   1,     1 },
        {   2, 34821 },
        {   5, 20482 },
        {  10, 10241 },
        {  15, 49159 },
        {  20, 39941 },
        {  25, 56100 },
        {  46,  2983 },
        {  66, 19094 },
        {  67, 56530 },
        {  87, 55062 },
        { 110, 61399 },
        { 132,  9547 },
        { 133, 63805 },
        { 144, 25710 },
        { 200, 37729 },
        { 242, 46215 },
        { 253, 38174 },
        { 254, 12174 },
        { 255,  7372 },
    };

// Invert prime2325
const expected_result<fifi::field::prime2325> results2325[] =
    {
        // arg1,       result
        {          1U,          1U },
        {          2U, 2147483646U },
        {  272572125U, 1887955081U },
        { 1351905135U, 4218730939U },
        { 1353929616U, 2406667616U },
        { 1444146446U,  811575873U },
        { 1630048742U, 3481329838U },
        { 1879748638U, 4180067651U },
        { 1909520067U,  527117708U },
        { 2001693299U, 2659441565U },
        { 2037479732U,  770197497U },
        { 2399533030U,  403603339U },
        { 2832328698U, 1639852349U },
        { 2968542519U, 2207640096U },
        { 3319742702U, 2900579629U },
        { 3477593642U, 2778157346U },
        { 3795789800U,  915126934U },
        { 3909187862U, 1505740374U },
        { 4175463158U, 2780563524U },
        { 4294967290U, 4294967290U },
    };

// clang-format on
}

TEST(test_invert, binary)
{
    constexpr fifi::field::binary field;

    for (auto result : results)
    {
        uint8_t actual = fifi::online::invert(field, result.m_input);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_invert, binary4)
{
    constexpr fifi::field::binary4 field(0b10011);

    for (auto result : results4)
    {
        uint8_t actual = fifi::online::invert(field, result.m_input);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_invert, binary8)
{
    constexpr fifi::field::binary8 field(0x11D);

    for (auto result : results8)
    {
        uint8_t actual = fifi::online::invert(field, result.m_input);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_invert, binary16)
{
    constexpr fifi::field::binary16 field(0x1100B);

    for (auto result : results16)
    {
        uint16_t actual = fifi::online::invert(field, result.m_input);

        uint16_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_invert, prime2325)
{
    constexpr fifi::field::prime2325 field;

    for (auto result : results2325)
    {
        uint32_t actual = fifi::online::invert(field, result.m_input);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}
