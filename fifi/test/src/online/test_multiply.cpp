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

#include <fifi/online/multiply.hpp>

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
        { 0, 0, 0 },
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 1, 1, 1 },
    };

const expected_result<fifi::field::binary4> results4[] =
    {
        // m_input1, m_input2, m_result
        {  0, 13,  0 },
        {  0, 14,  0 },
        {  1,  1,  1 },
        {  2,  9,  1 },
        {  2, 11,  5 },
        {  3,  2,  6 },
        {  4,  8,  6 },
        {  5, 11,  1 },
        {  5, 14,  3 },
        {  5, 15,  6 },
        {  6,  6,  7 },
        {  6, 11, 15 },
        {  7, 15, 11 },
        {  8, 12, 10 },
        {  9,  5, 11 },
        {  9,  9, 13 },
        { 10,  0,  0 },
        { 11,  9, 12 },
        { 12,  4,  5 },
        { 12,  8, 10 },
        { 13,  1, 13 },
        { 13, 15,  7 },
        { 14,  7, 12 },
        { 14,  8,  9 },
        { 15,  5,  6 },
    };

const expected_result<fifi::field::binary8> results8[] =
    {
        // m_input1, m_input2, m_result
        {   0,   0,   0 },
        {   0,   2,   0 },
        {   1,   1,   1 },
        {   1,   2,   2 },
        {   1,  46,  46 },
        {   2,   1,   2 },
        {   4, 200,   7 },
        {   5,   5,  17 },
        {  10,  10,  68 },
        {  10,  20, 136 },
        {  12,  87, 195 },
        {  25,  10, 250 },
        {  25,  15, 135 },
        {  25,  66,  60 },
        {  30, 242,  78 },
        {  67,   1,  67 },
        { 110,   7,  23 },
        { 110, 253, 232 },
        { 144,  66, 206 },
        { 176, 132, 204 },
        { 200, 200, 158 },
        { 251, 133, 104 },
        { 254, 254, 227 },
        { 255, 255, 226 },
    };

const expected_result<fifi::field::binary16> results16[] =
    {
        // m_input1, m_input2, m_result
        {   0,     0,       0 },
        {   0,     2,       0 },
        {   1,     1,       1 },
        {   1,     2,       2 },
        {   1,    46,      46 },
        {   2,     1,       2 },
        {   4,   200,     800 },
        {   5,     5,      17 },
        {  10,    10,      68 },
        {  10,    20,     136 },
        {  12,    87,     996 },
        {  25,    10,     250 },
        {  25,    15,     135 },
        {  25,    66,    1650 },
        {  30,   242,    2716 },
        {  67,     1,      67 },
        { 110,     7,     266 },
        { 110,   253,    9478 },
        { 144,    66,    9504 },
        { 176,   132,   23232 },
        { 200,   200,   20544 },
        { 251,   133,   32407 },
        { 254,   254,   21844 },
        { 255,   255,   21845 },
        { 65535, 65535, 1843  },
        { 65535, 1,     65535 },
        { 65535, 2196,  1     }
    };

const expected_result<fifi::field::prime2325> results2325[] =
    {
        // arg1,       arg2,        result
        {          0U,          0U,          0U },
        {    8961698U, 1454316048U, 2844647930U },
        {  188377222U, 2440371958U, 1837383177U },
        {  366785572U, 3273833947U, 1357321858U },
        {  484124441U, 2513732051U, 2038062075U },
        {  549534826U, 3692094631U, 3813011302U },
        {  591588585U, 2298808855U, 2016341846U },
        {  628824443U, 2882587157U, 2414550839U },
        {  714307256U, 4017788735U, 2552564636U },
        {  890246292U, 3115294715U, 3374921530U },
        { 1135999486U, 2298616897U, 2948135396U },
        { 1241752606U, 2682856520U, 1016247841U },
        { 1319528055U, 3213225118U,  472935279U },
        { 1501780748U,  772682526U,  464840319U },
        { 1516764886U, 2487733786U, 1221426309U },
        { 1921500106U, 1581124167U, 2064202209U },
        { 1923821210U,    4910817U, 1727198145U },
        { 2747836187U,  606517521U, 4005335094U },
        { 2755140095U, 3176782815U, 3071678684U },
        { 2865873554U, 4023346111U, 1390523154U },
        { 2898832361U, 3285799624U,  205888686U },
        { 3216464703U, 2321361148U, 2159784831U },
        { 3301510135U, 2380300242U, 3460816498U },
        { 3791215484U,  823219598U, 3413971131U },
        { 3916985401U, 2885578550U, 3986996714U },
        { 3948329266U, 1846670630U, 2958898160U },
        { 4057832578U,  783228777U, 1061113545U },
        { 4071004727U, 3893610017U, 3098938562U },
        { 4294967290U,          1U, 4294967290U },
        { 4294967290U,          2U, 4294967289U },
        { 4294967290U, 4294967290U,          1U },
    };

// clang-format on
}

TEST(test_multiply, binary)
{
    constexpr fifi::field::binary field;

    for (auto result : results)
    {
        uint8_t actual =
            fifi::online::multiply(field, result.m_input1, result.m_input2);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_multiply, binary4)
{
    constexpr fifi::field::binary4 field(0b10011);

    for (auto result : results4)
    {
        uint8_t actual =
            fifi::online::multiply(field, result.m_input1, result.m_input2);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_multiply, binary8)
{
    constexpr fifi::field::binary8 field(0x11D);

    for (auto result : results8)
    {
        uint8_t actual =
            fifi::online::multiply(field, result.m_input1, result.m_input2);

        uint8_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_multiply, binary16)
{
    constexpr fifi::field::binary16 field(0x1100B);

    for (auto result : results16)
    {
        uint16_t actual =
            fifi::online::multiply(field, result.m_input1, result.m_input2);

        uint16_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}

TEST(test_multiply, prime2325)
{
    constexpr fifi::field::prime2325 field;

    for (auto result : results2325)
    {
        uint32_t actual =
            fifi::online::multiply(field, result.m_input1, result.m_input2);

        uint32_t expected = result.m_result;

        EXPECT_EQ(expected, actual);
    }
}
