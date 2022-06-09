// Copyright Steinwurf ApS 2012.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "../version.hpp"

#include <cassert>
#include <cstdint>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
/// Ceiling for integer division - ceil(numerator/denominator)
/// @param numerator the division numerator
/// @param denominator the division denominator
/// @return returns the ceiled result of integer division
inline std::size_t ceil_division(std::size_t numerator, std::size_t denominator)
{
    assert(denominator > 0);

    if (numerator == 0)
        return 0;

    return ((numerator - 1) / denominator) + 1;
}
}
}
}
