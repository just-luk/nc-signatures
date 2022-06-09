// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>

#include "../byte_count.hpp"
#include "../symbol_count.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
template <class Super>
class layer_coefficients_bytes : public Super
{
public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_max_coefficients_bytes = Super::elements_to_bytes(Super::symbols());
    }

    /// Return the maximum number of bytes needed for a coefficient vector.
    auto max_coefficients_bytes() const -> byte_count
    {
        assert(m_max_coefficients_bytes > byte_count{0});

        return m_max_coefficients_bytes;
    }

private:
    /// The maximum size of the coefficient vector
    byte_count m_max_coefficients_bytes{0};
};
}
}
}
}
