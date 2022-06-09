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

namespace helper
{

namespace details
{
template <class F>
struct wrapper_vector_dot_product
{
    wrapper_vector_dot_product(F f) : m_f(f)
    {
    }

    template <class... Args>
    uint32_t vector_dot_product(Args&&... args) const
    {
        return m_f(std::forward<Args>(args)...);
    }

    F m_f;
};
}

/// Takes a callable and returns a object with the vector_dot_product method
template <class F>
inline details::wrapper_vector_dot_product<F> wrap_vector_dot_product(F f)
{
    return details::wrapper_vector_dot_product<F>(f);
}

}
