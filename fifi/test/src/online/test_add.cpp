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

#include <fifi/online/add.hpp>

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
        {   13775737U, 3832303341U, 3846079078U },
        {  416764808U, 2952672146U, 3369436954U },
        {  954547203U, 3403230939U,   62810851U },
        { 1304171116U, 2058347943U, 3362519059U },
        { 1997027658U, 1158755251U, 3155782909U },
        { 2228876704U, 3769080616U, 1702990029U },
        { 2699559732U, 4130167990U, 2534760431U },
        { 2721360764U, 3099915689U, 1526309162U },
        { 2761881503U, 3727769459U, 2194683671U },
        { 2780503337U,  508086692U, 3288590029U },
        { 3164597666U,  249577394U, 3414175060U },
        { 3275759048U,  318206772U, 3593965820U },
        { 3393423153U, 2569499151U, 1667955013U },
        { 3549012440U, 3097460608U, 2351505757U },
        { 3562900696U,     433428U, 3563334124U },
        { 3733613168U, 4250233244U, 3688879121U },
        { 3860097427U, 3766812989U, 3331943125U },
        { 4294967290U,          1U,          0U },
        { 4294967290U,          2U,          1U },
        { 4294967290U, 4294967290U, 4294967289U },
    };

// clang-format on

template <class Field>
inline void test_binary_add(const Field& field)
{
    for (uint32_t i = 0; i < 100; ++i)
    {
        uint32_t a = helper::random_value(field);
        uint32_t b = helper::random_value(field);

        EXPECT_EQ(a ^ b, fifi::online::add(field, a, b));
    }
}
}

TEST(test_add, binary)
{
    test_binary_add(fifi::field::binary());
}

TEST(test_add, binary4)
{
    test_binary_add(fifi::field::binary4());
}

TEST(test_add, binary8)
{
    test_binary_add(fifi::field::binary8());
}

TEST(test_add, binary16)
{
    test_binary_add(fifi::field::binary16());
}

TEST(test_add, prime2325)
{
    constexpr fifi::field::prime2325 field;

    for (auto result : results2325)
    {
        uint32_t actual =
            fifi::online::add(field, result.m_input1, result.m_input2);

        EXPECT_EQ(result.m_result, actual);
    }
}
