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

#pragma once

#include <cstring>
#include <vector>

#include "bytes_to_elements.hpp"
#include "get_value.hpp"

#include "../version.hpp"
#include "prime2325.hpp"
#include "prime2325_prefix_length.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Bitmap algorithm for finding the an unused bit prefix within a block of
/// data, using the prefix we may map arbitrary data to the 2^32 - 5 prime
/// field a approach which was suggested by Crowley et al.
struct prime2325_bitmap
{
    /// The data type used for the bitmap
    using mapping_type = uint32_t;

    /// Create a new prefix bitmap
    ///
    /// @param max_block_size denotes the largest block size in bytes that
    ///        can be searched using this algorithm.
    prime2325_bitmap(std::size_t max_block_size) :
        m_max_block_size(max_block_size),
        m_max_block_length(bytes_to_elements(prime2325(), max_block_size))
    {
        assert(m_max_block_length <= prime2325::max_block_length);
        assert(m_max_block_length > 0);

        // log2 of the maximum block size gives us the
        // maximum bit prefix needed
        uint32_t max_prefix_bits = prime2325_prefix_length(m_max_block_length);

        // The amount to shift the prefix to get the prefix value:
        //
        // | b31 b30 b29 b28 b27  ... b1 b0 |
        //    ^               ^
        //    '-   prefix    -'
        //          value
        //
        // In the above example we have 5 prefix bits so we need to
        // shift it down 32-5 = 27 bits to get the numeric prefix value

        m_shift_prefix = 32 - max_prefix_bits;

        // The number of bits in the mapping type
        m_mapping_bits = std::numeric_limits<mapping_type>::digits;

        // We need one bit per possible prefix value we have
        // 2^m_max_prefix_bits possible values
        std::size_t bytes = size_needed(m_max_block_length);

        // ceil(x/y) = ((x - 1) / y) + 1
        std::size_t bitmap_elements = ((bytes - 1) / sizeof(mapping_type)) + 1;
        m_bitmap.resize(bitmap_elements);
    }

    /// @param block_length the length of a block
    /// @return the size in bytes needed for the bitmap depending on the
    /// block length i.e. the number of 32-bit integers in the block.
    static std::size_t size_needed(std::size_t block_length)
    {
        assert(block_length > 0);
        assert(block_length <= prime2325::max_block_length);

        uint32_t prefix_bits = prime2325_prefix_length(block_length);

        // ceil(x/y) = ((x - 1) / y) + 1
        std::size_t size_bytes = (((1 << prefix_bits) - 1) / 8) + 1;
        assert(size_bytes > 0);

        return size_bytes;
    }

    /// Finds a returns an unused prefix in binary data.
    /// @param data The data buffer to search
    /// @param size The number of bytes in the data buffer
    /// @return An unused prefix
    uint32_t find_prefix(const uint8_t* data, std::size_t size)
    {
        assert(size <= m_max_block_size);
        assert(size > 0);

        // Size must be multiple of 4 bytes due to the field 2^32-5
        assert(size % 4 == 0);
        assert(data != nullptr);

        std::fill(m_bitmap.begin(), m_bitmap.end(), 0);

        std::size_t elements = bytes_to_elements(prime2325(), size);

        for (std::size_t i = 0; i < elements; ++i)
        {
            uint32_t value = 0;
            std::memcpy(&value, data + (i * sizeof(value)), sizeof(value));

            update_bitmap(value);
        }

        uint32_t prefix = 0;
        bool found = find_in_bitmap(&prefix);

        assert(found);
        (void)found;

        return prefix;
    }

    /// Iterates through the bitmap looking for the unused prefix
    /// @param prefix will contain the unused prefix
    /// @return returns true if the prefix was found otherwise false
    bool find_in_bitmap(uint32_t* prefix) const
    {
        // Find the missing prefix in the bitmap
        for (std::size_t i = 0; i < m_bitmap.size(); ++i)
        {
            for (std::size_t j = 0; j < m_mapping_bits; ++j)
            {
                if ((m_bitmap[i] & (1 << j)) == 0)
                {
                    *prefix = (i * m_mapping_bits + j) << m_shift_prefix;
                    return true;
                }
            }
        }

        return false;
    }

    /// Updates the bitmap so that it reflects that the value has been found
    void update_bitmap(uint32_t value)
    {
        // We use a 32 bit data type for the base value
        uint32_t prefix_value = value >> m_shift_prefix;

        std::size_t index = prefix_value / m_mapping_bits;
        std::size_t offset = prefix_value % m_mapping_bits;

        m_bitmap[index] = m_bitmap[index] | 1 << offset;
    }

    /// The maximum block size in bytes
    std::size_t m_max_block_size;

    /// The maximum block length in elements of uint32_t
    std::size_t m_max_block_length;

    // The number of bits in the mapping type
    std::size_t m_mapping_bits;

    /// The amount we need to shift the prefix
    uint32_t m_shift_prefix;

    /// The bitmap storing info about whether a specific prefix was found
    std::vector<mapping_type> m_bitmap;
};

}
}
}
