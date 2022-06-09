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

#include <fifi/detail/log_table.hpp>
#include <fifi/detail/table_data.hpp>
#include <fifi/online/subtract.hpp>

#include "symbol_index.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// Cauchy RS generator matrix for systematic coding
/// To construct an m x n Cauchy matrix, we need to define X and Y that are
/// sets of GF(2^8) values that do not intersect.
/// X contains m values and Y contains n values, then each element of the
/// Cauchy matrix is defined as:
///
///     Aij = 1 / (Xi - Yj)
///
/// To obtain a matrix structure that is reusable for all values of g=[1..255],
/// we define X = [255,...,g] and Y = [0,1,2,...,g-1].
///
/// The resulting layout of X and Y when calculating the matrix elements:
///
///                      Y
///          0   1   2   3  ...  g-1
///     255
///     254
///     253
///  X  .
///     .
///     .
///     g
///
/// We can use this construction to pre-calculate a full 255x255 matrix,
/// then we can take the top-left submatrix that is appropriate for the
/// current value of g.
/// For example, if g=5 then the coefficients from the 251x5 top-left submatrix
/// will be used. We can send 251 linearly independent repair symbols after
/// the 5 original symbols.
///
/// This Cauchy matrix structure was inspired by the comments here:
/// https://github.com/catid/cm256/blob/master/cm256.cpp
template <class Field>
struct rs_cauchy_matrix_backend
{
    constexpr rs_cauchy_matrix_backend() noexcept : m_matrix()
    {
        fifi::detail::log_table<Field> lookup_table(Field{});
        for (std::size_t i = 0; i < Field::max_value; ++i)
        {
            uint8_t xi = Field::max_value - i;

            for (std::size_t j = 0; j < Field::max_value; ++j)
            {
                auto result = fifi::online::subtract(Field{}, xi, j);
                if (result != 0)
                    m_matrix[i][j] = lookup_table.invert(result);
            }
        }
    }

    const uint8_t* row(symbol_index index) const
    {
        assert(index.value < Field::max_value);
        return m_matrix[index.value];
    }

private:
    fifi::detail::table_data<uint8_t, Field::max_value, Field::max_value>
        m_matrix;
};
}
}
}
