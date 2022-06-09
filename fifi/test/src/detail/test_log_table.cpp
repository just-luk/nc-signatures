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

#include <fifi/detail/log_table.hpp>

#include <gtest/gtest.h>
#include <type_traits>

#include <fifi/field/binary16.hpp>
#include <fifi/field/binary4.hpp>
#include <fifi/field/binary8.hpp>

#include "../helper/test_invert.hpp"
#include "../helper/test_multiply.hpp"

using namespace fifi::field;

TEST(test_log_table, binary4_invert)
{
    fifi::detail::log_table<binary4> math{binary4(0x13)};

    helper::test_invert(math);
}

TEST(test_log_table, binary4_multiply)
{
    fifi::detail::log_table<binary4> math{binary4(0x13)};

    helper::test_multiply(math);
}

TEST(test_log_table, binary8_invert)
{
    fifi::detail::log_table<binary8> math{binary8(0x11D)};

    helper::test_invert(math);
}

TEST(test_log_table, binary8_multiply)
{
    fifi::detail::log_table<binary8> math{binary8(0x11D)};

    helper::test_multiply(math);
}

TEST(test_log_table, binary16_invert)
{
    fifi::detail::log_table<binary16> math{binary16(0x1100B)};

    helper::test_invert(math);
}

TEST(test_log_table, binary16_multiply)
{
    fifi::detail::log_table<binary16> math{binary16(0x1100B)};

    helper::test_multiply(math);
}
