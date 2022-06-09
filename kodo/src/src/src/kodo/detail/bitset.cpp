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

#include <algorithm>
#include <cstring>

#include "bitset.hpp"
#include "ceil_division.hpp"
#include "reverse.hpp"
#include "to_bit.hpp"
#include "to_bit_range.hpp"
#include "to_bitmask.hpp"
#include "to_byte_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

bitset::bitset()
{
}

bitset::bitset(bit_count bits) : m_bits(bits.value)
{
    resize(bits);
}

auto bitset::bits() const -> bit_count
{
    return bit_count{m_bits};
}

void bitset::resize(bit_count bits)
{
    assert(bits.value > 0);

    m_bits = bits.value;
    m_ones = bit_count{0};
    m_msb = bit_index{0};
    m_lsb = bit_index{0};

    std::size_t storage_needed = ceil_division(m_bits, 8);
    assert(storage_needed > 0);
    m_storage.resize(storage_needed);
    std::fill(m_storage.begin(), m_storage.end(), false);
}

void bitset::set(bit_index index)
{
    assert(get(index) == false);

    /// Update LSB, MSB and count
    if (m_ones > bit_count{0})
    {
        m_lsb = std::min(m_lsb, index);
        m_msb = std::max(m_msb, index);
    }
    else
    {
        m_lsb = index;
        m_msb = index;
    }

    ++m_ones;

    byte_index offset = to_byte_index(index);
    uint8_t mask = to_bitmask(index);

    m_storage[offset.value] |= mask;
}

auto bitset::get(bit_index index) const -> bool
{
    byte_index offset = to_byte_index(index);
    uint8_t mask = to_bitmask(index);

    return m_storage[offset.value] & mask;
}

void bitset::unset(bit_index index)
{
    assert(get(index) == true);

    if (m_ones == bit_count{1})
    {
        // No more bits
        m_lsb = bit_index{0};
        m_msb = bit_index{0};
    }
    else if (m_ones == bit_count{2})
    {
        // One bit left so lsb == msb
        m_lsb = index == m_lsb ? m_msb : m_lsb;
        m_msb = m_lsb;
    }
    else if (index == m_lsb)
    {
        // The range skips the current lsb and also the current msb. Since the
        // new lsb should be found in-between those two
        m_lsb = find_lsb({m_lsb + bit_count{1}, m_msb});
    }
    else if (index == m_msb)
    {
        // The range skips the current msb and lsb. Since the new msb should
        // be found in-between those two
        m_msb = find_msb({m_lsb + bit_count{1}, m_msb});
    }

    --m_ones;

    byte_index offset = to_byte_index(index);
    uint8_t mask = to_bitmask(index);

    m_storage[offset.value] &= ~mask;
}

auto bitset::count_ones() const -> bit_count
{
    return m_ones;
}

auto bitset::msb() const -> bit_index
{
    assert(m_ones > bit_count{0});
    return m_msb;
}

auto bitset::lsb() const -> bit_index
{
    assert(m_ones > bit_count{0});
    return m_lsb;
}

auto bitset::lower_bound() const -> bit_index
{
    return bit_index{0};
}

auto bitset::upper_bound() const -> bit_index
{
    return bit_index{m_bits};
}

bit_index bitset::find_lsb(const bit_range& range)
{
    /// Todo use optional
    bit_index index{0};
    bool found = false;

    for (bit_index i : range)
    {
        byte_index offset = to_byte_index(i);
        uint8_t mask = to_bitmask(i);

        if (m_storage[offset.value] & mask)
        {
            index = i;
            found = true;
            break;
        }
    }

    assert(found);
    (void)found;
    return index;
}

bit_index bitset::find_msb(const bit_range& range)
{
    /// Todo use optional
    bit_index index{0};
    bool found = false;

    for (bit_index i : reverse(range))
    {
        byte_index offset = to_byte_index(i);
        uint8_t mask = to_bitmask(i);

        if (m_storage[offset.value] & mask)
        {
            index = i;
            found = true;
            break;
        }
    }

    assert(found);
    (void)found;
    return index;
}

auto bitset::storage() const -> const uint8_t*
{
    return m_storage.data();
}

auto bitset::storage_bytes() const -> byte_count
{
    return byte_count{m_storage.size()};
}

}
}
}
