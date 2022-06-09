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

#include <fifi/field/binary8.hpp>

#include "symbol_index.hpp"

#include "rs_cauchy_matrix_backend.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
template <class Field>
struct rs_cauchy_matrix
{
    static_assert(std::is_same<Field, fifi::field::binary4>::value ||
                      std::is_same<Field, fifi::field::binary8>::value,
                  "Only binary4 and binary8 is allowed.");

    /// Constructor
    rs_cauchy_matrix();

    const uint8_t* row(symbol_index index) const
    {
        return m_backend->row(index);
    }

private:
    const rs_cauchy_matrix_backend<Field>* m_backend;
};

template <>
rs_cauchy_matrix<fifi::field::binary4>::rs_cauchy_matrix();
template <>
rs_cauchy_matrix<fifi::field::binary8>::rs_cauchy_matrix();
}
}
}
