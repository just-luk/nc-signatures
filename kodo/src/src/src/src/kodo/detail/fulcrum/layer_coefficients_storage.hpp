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
#include <cstring>
#include <vector>

#include "../symbol_count.hpp"

#include "../../block/generator/random_uniform.hpp"
#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{

template <class Super>
struct layer_coefficients_storage : public Super
{
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_generator = {Super::field()};
        m_generator.configure(Super::symbols().value);
        m_coefficients.resize(m_generator.max_coefficients_bytes());
    }

    /// Given a index from the inner code, return a pointer to the coefficients
    /// in the outer code.
    /// @param index The index of the inner symbol
    uint8_t* outer_coefficients(symbol_index index)
    {
        assert(index < Super::inner_symbols());

        if (index < Super::symbols())
        {
            // Zero the coefficients buffer
            std::memset(m_coefficients.data(), 0, m_coefficients.size());
            Super::set_value(m_coefficients.data(), index, 1U);
        }
        else
        {
            // Get the offset into the expansion
            symbol_index offset = index - Super::symbols();

            assert(offset < Super::expansion());
            m_generator.set_seed(offset.value);
            m_generator.generate(m_coefficients.data());
        }
        return m_coefficients.data();
    }

private:
    kodo::block::generator::random_uniform m_generator;
    /// Temporary buffer coefficients of the outer code
    std::vector<uint8_t> m_coefficients;
};
}
}
}
}
