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
#include <cstdint>
#include <cstring>
#include <vector>

#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
/// @brief Provides storage and access to the coding coefficients
///        used during encoding and decoding.
template <class Super>
class layer_coefficients_storage : public Super
{
public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        auto symbols = config.symbols;
        assert(symbols != symbol_count{0});
        m_coefficients_storage.resize(symbols.value);

        for (auto& storage : m_coefficients_storage)
        {
            storage.resize(Super::max_coefficients_bytes().value);
        }
    }

    uint8_t* coefficients_data(symbol_index index)
    {
        assert(index <= Super::last_symbol());
        return m_coefficients_storage[index.value].data();
    }

    const uint8_t* coefficients_data(symbol_index index) const
    {
        assert(index <= Super::last_symbol());
        return m_coefficients_storage[index.value].data();
    }

    void copy_into_coefficients(symbol_index index, const uint8_t* src)
    {
        assert(index <= Super::last_symbol());
        std::memcpy(coefficients_data(index), src,
                    Super::max_coefficients_bytes().value);
    }

private:
    /// Stores the encoding vectors, we do a vector of vectors
    /// to maximize chances of proper alignment of individual
    /// coefficient vectors. This is needed when using SSE etc.
    /// instructions for fast computations with the coefficients
    std::vector<std::vector<uint8_t>> m_coefficients_storage;
};
}
}
}
}
