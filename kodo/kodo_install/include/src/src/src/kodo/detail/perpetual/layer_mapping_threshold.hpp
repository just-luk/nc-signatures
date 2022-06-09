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
#include <cstring>
#include <vector>

#include "../symbol_count.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{
template <class Super>
struct layer_mapping_threshold : public Super
{
    /// The configuration options for this layer
    struct config : public Super::config
    {
        /// Mapping threshold - how much of the inner code we need to fill
        /// before we map to the outer code
        double mapping_threshold = 0;
    };

    /// Configure the layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        assert(config.mapping_threshold <= 1.0);
        assert(config.mapping_threshold > 0.0);

        m_mapping_threshold = config.mapping_threshold;

        // Calculate the number of data symbol pivots we need in the inner code.
        // The number of data symbols missing will be the "holes" we need to fix
        // with the outer code.
        m_symbol_mapping_threshold = symbol_count{
            std::max(std::size_t{1},
                     static_cast<std::size_t>(Super::data_symbols().value *
                                              m_mapping_threshold))};

        assert(m_symbol_mapping_threshold <= Super::data_symbols());
    }

    /// Return true when we want to map all the outer code symbols from the
    /// inner decoder to the outer decoder.
    auto should_map_to_outer() const -> bool
    {
        return Super::inner_data_rank() > m_symbol_mapping_threshold;
    }

    /// Return the mapping threshold in symbols. The mapping threshold
    /// determines how "full" the inner decoder should be before we map the
    /// outer symbols to the outer decoder.
    auto mapping_threshold() const -> double
    {
        return m_mapping_threshold;
    }

    /// Return the mapping threshold in symbols. The mapping threshold
    /// determines how "full" the inner decoder should be before we map the
    /// outer symbols to the outer decoder.
    auto symbol_mapping_threshold() const -> symbol_count
    {
        return m_symbol_mapping_threshold;
    }

private:
    /// The mapping threshold
    double m_mapping_threshold{0};

    /// The mapping threshold in symbols
    symbol_count m_symbol_mapping_threshold{0};
};

}
}
}
}
