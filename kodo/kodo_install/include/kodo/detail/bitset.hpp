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

#include <cassert>
#include <vector>

#include "../version.hpp"

#include "bit_count.hpp"
#include "bit_index.hpp"
#include "bit_range.hpp"
#include "byte_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
/// Simple class for storing an array of bits
class bitset
{
public:
    /// Construct a bitset
    bitset();

    /// Create a bitset with a given capacity
    bitset(bit_count bits);

    /// Return the current capacity in number of bits
    auto bits() const -> bit_count;

    /// Resize the bitset to be able to store the specified number of bits
    void resize(bit_count bits);

    /// Set a specific bit to one
    void set(bit_index index);

    /// Set a specific bit to zero
    void unset(bit_index index);

    /// Get value of the bit at the specified index
    auto get(bit_index index) const -> bool;

    /// Return the number of bits set to true
    auto count_ones() const -> bit_count;

    /// Get the position of the least significant bit
    auto lsb() const -> bit_index;

    /// Get the position of the most significant bit
    auto msb() const -> bit_index;

    /// The lower bound of the bitset (included)
    auto lower_bound() const -> bit_index;

    /// The uppper bound of the bitset (excluded)
    auto upper_bound() const -> bit_index;

    /// The data pointer to the internal storage object of the bitset
    auto storage() const -> const uint8_t*;

    /// The size of the internal storage object of the bitset
    auto storage_bytes() const -> byte_count;

private:
    /// Find the LSB in the bitrange
    bit_index find_lsb(const bit_range& range);

    /// Find the MSB in the bitrange
    bit_index find_msb(const bit_range& range);

private:
    /// The number of bits stored
    std::size_t m_bits = 0;

    /// Number of bits ON
    bit_count m_ones{0};

    /// THe MSB
    bit_index m_msb{0};

    /// The LSB
    bit_index m_lsb{0};

    /// Backing storage for the bits
    std::vector<uint8_t> m_storage;
};

}
}
}
