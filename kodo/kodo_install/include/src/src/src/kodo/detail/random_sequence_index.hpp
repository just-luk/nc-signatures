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

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ostream>
#include <random>
#include <vector>

#include <ranbo/xoshiro256ss.h>

#include "../version.hpp"

#include "symbol_count.hpp"
#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
struct random_sequence_index
{
    struct field
    {
        uint32_t prime{0};
        uint32_t degree{0};
    };

    random_sequence_index()
    {
    }

    random_sequence_index(symbol_count symbols) :
        m_symbols(symbols), m_field(select_field(symbols)), m_index(1U)
    {
        set_seed(0);
    }

    random_sequence_index(const random_sequence_index&) = default;
    random_sequence_index& operator=(const random_sequence_index&) = default;

    void set_seed(uint32_t seed)
    {
        m_index = 1;

        ranbo_xoshiro256ss_set_seed(&m_generator, seed);
        m_randomize = ranbo_xoshiro256ss_generate(&m_generator);
    }

    void advance()
    {
        do
        {
            m_index = (0b1 << (m_field.degree - 1)) & m_index
                          ? (m_index << 1) ^ m_field.prime
                          : m_index << 1;

        } while (m_index > m_symbols.value);

        if (m_index == 1)
        {
            m_randomize = ranbo_xoshiro256ss_generate(&m_generator);
        }
    }

    auto index() const -> symbol_index
    {
        // We need to have zero part of the range
        return symbol_index{((m_index - 1U) + m_randomize) % m_symbols.value};
    }

    static auto select_degree(symbol_count symbols) -> uint32_t
    {
        std::vector<uint32_t> degrees{
            0b1 << 1,  0b1 << 2,  0b1 << 3,  0b1 << 4,  0b1 << 5,
            0b1 << 6,  0b1 << 7,  0b1 << 8,  0b1 << 9,  0b1 << 10,
            0b1 << 11, 0b1 << 12, 0b1 << 13, 0b1 << 14, 0b1 << 15,
            0b1 << 16, 0b1 << 17, 0b1 << 18, 0b1 << 19, 0b1 << 20,
            0b1 << 21, 0b1 << 22, 0b1 << 23, 0b1 << 24, 0b1 << 25};

        return *std::upper_bound(degrees.begin(), degrees.end(), symbols.value);
    }

    static auto select_field(symbol_count symbols) -> field
    {
        uint32_t degree = select_degree(symbols);

        switch (degree)
        {
        case 0b1 << 1:
            // x^1
            return field{0b10, 1};
        case 0b1 << 2:
            // x^2 + x^1 + 1
            return field{0b111, 2};
        case 0b1 << 3:
            // x^3 + x^1 + 1
            return field{0b1011, 3};
        case 0b1 << 4:
            // x^4 + x^1 + 1
            return field{0b10011, 4};
        case 0b1 << 5:
            // x^5 + x^2 + 1
            return field{0b100101, 5};
        case 0b1 << 6:
            // x^6 + x^1 + 1
            return field{0b1000011, 6};
        case 0b1 << 7:
            // x^7 + x^1 + 1
            return field{0b10000011, 7};
        case 0b1 << 8:
            // x^8 + x^4 + x^3 + x^2 + 1
            return field{0b100011101, 8};
        case 0b1 << 9:
            // x^9 + x^4 + 1
            return field{0b1000010001, 9};
        case 0b1 << 10:
            // x^10 + x^3 + 1
            return field{0b10000001001, 10};
        case 0b1 << 11:
            // x^11 + x^2 + 1
            return field{0b100000000101, 11};
        case 0b1 << 12:
            // x^12 + x^6 + x^4 + x^1 + 1
            return field{0b1000001010011, 12};
        case 0b1 << 13:
            // x^13 + x^4 + x^3 + x^1 + 1
            return field{0b10000000011011, 13};
        case 0b1 << 14:
            // x^14 + x^8 + x^6 + x^1 + 1
            return field{0b100000101000011, 14};
        case 0b1 << 15:
            // x^15 + x^1 + 1
            return field{0b1000000000000011, 15};
        case 0b1 << 16:
            // x^16 + x^12 + x^3 + x^1 + 1
            return field{0b10001000000001011, 16};
        case 0b1 << 17:
            // x^17 + x^3 + 1
            return field{0b100000000000001001, 17};
        case 0b1 << 18:
            // x^18 + x^7 + 1
            return field{0b1000000000010000001, 18};
        case 0b1 << 19:
            // x^19 + x^5 + x^2 + x^1 + 1
            return field{0b10000000000000100111, 19};
        case 0b1 << 20:
            // x^20 + x^3 + 1
            return field{0b100000000000000001001, 20};
        case 0b1 << 21:
            // x^21 + x^2 + 1
            return field{0b1000000000000000000101, 21};
        case 0b1 << 22:
            // x^22 + x^1 + 1
            return field{0b10000000000000000000011, 22};
        case 0b1 << 23:
            // x^23 + x^5 + 1
            return field{0b100000000000000000100001, 23};
        case 0b1 << 24:
            // x^24 + x^7 + x^2 + x^1 + 1
            return field{0b1000000000000000010000111, 24};
        case 0b1 << 25:
            // x^25 + x^3 + 1
            return field{0b10000000000000000000001001, 25};
        default:
            break;
        }

        assert(0 && "Not valid symbols");
        return field{};
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;

    symbol_count m_symbols{0};
    field m_field{};
    uint32_t m_index{0};
    uint32_t m_randomize{0};
};
}
}
}
