// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>
#include <cstdint>

#include "../../version.hpp"
#include "../byte_count.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace fulcrum
{
/// Building block for creating an "elimination decoder".
/// This layer keeps track of the offset which is added when
/// accessing the coefficients in the coefficients vector.
///
/// This layer can be inserted into a decoding stack to effectively
/// create a non-square decoding matrix - which has more columns than rows.
/// In the following example, we have matrix C which is a 3x6 matrix, we
/// can consider this as two 3x3 sub-matrices:
///
///       C
///  <--------->
///    A     B
///  <---> <--->
///  0 1 1 1 0 0
///  0 0 1 0 1 0
///  1 1 0 0 0 1
///
/// In this case, we consider the matrix A as the expansion.
/// You will notice that matrix B is full rank and can be used to
/// eliminate the last 3 coefficients of any incoming encoding vectors.
///
/// We can shift these pivot positions to other columns by applying an offset.
/// For example, when the decoding algorithm accesses the coefficient at
/// index 0, this layer can adjust that to mean index 3 (if the offset value
/// is set to 3).
template <class Super>
class layer_elemination_offset : public Super
{
public:
    struct config : public Super::config
    {
        symbol_count elimination_offset{0};
    };

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);

        auto symbols = config.symbols;
        assert(symbols != symbol_count{0});
        m_offset = config.elimination_offset;
        m_max_coefficients_bytes = Super::elements_to_bytes(m_offset + symbols);
    }

    auto elemination_offset_range() const -> detail::symbol_range
    {
        return detail::symbol_range(
            symbol_index{0},
            symbol_index{m_offset.value + Super::symbols().value});
    }

    auto elemination_offset() const -> symbol_count
    {
        return m_offset;
    }

    /// Return the number of bytes needed for a coefficient vector.
    auto max_coefficients_bytes() const -> byte_count
    {
        assert(m_max_coefficients_bytes > byte_count{0});

        return m_max_coefficients_bytes;
    }

private:
    /// The offset to be used
    symbol_count m_offset{0};

    /// The size of the coefficient vector
    byte_count m_max_coefficients_bytes{0};
};
}
}
}
}
