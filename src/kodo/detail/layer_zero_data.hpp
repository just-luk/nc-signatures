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

#include <cstring>

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

template <class Super>
class layer_zero_data : public Super
{
public:
    /// Zero the data.
    /// @param data The data buffer containing the buffer to zero.
    /// @param bytes The number of bytes to zero.
    void zero_data(uint8_t* data, byte_count bytes) const
    {
        assert(data != nullptr);
        assert(bytes > byte_count{0U});
        std::memset(data, 0, bytes.value);
    }
};
}
}
}
