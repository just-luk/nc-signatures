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

#include <sstream>
#include <string>
#include <vector>

#include <fifi/field/get_value.hpp>

#include "../version.hpp"

#include "is_empty.hpp"
#include "is_last.hpp"
#include "relative_index.hpp"
#include "to_symbol_frame.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

template <class T>
inline std::string to_string(const T& s)
{
    std::stringstream ss;
    ss << s;
    return ss.str();
}

inline std::string to_string(const char* s)
{
    return s;
}

inline std::string to_string(const std::string& s)
{
    return s;
}

template <class T, class A>
inline std::string to_string(const std::vector<T, A>& v)
{
    if (v.empty())
    {
        return "[]";
    }

    std::stringstream s;
    s << "[";
    s << *v.begin();

    for (auto it = std::next(v.begin()); it != v.end(); ++it)
    {
        s << ", ";
        s << *it;
    }
    s << "]";

    return s.str();
}

template <class Field>
struct log_coefficients
{
    const uint8_t* data;
    symbol_range range;
};

template <class Field>
inline std::string to_string(const log_coefficients<Field>& coefficients)
{
    symbol_frame frame = to_symbol_frame(Field{}, coefficients.range);

    std::stringstream s;

    symbol_range left{frame.lower_bound(), coefficients.range.lower_bound()};
    symbol_range right{coefficients.range.upper_bound(), frame.upper_bound()};

    auto print = [&](const symbol_range& range)
    {
        s << "[";

        for (symbol_index i : range)
        {
            s << (uint32_t)fifi::field::get_value(
                Field{}, coefficients.data, relative_index(frame, i).value);

            if (!is_last(range, i))
            {
                s << ", ";
            }
        }
        s << "]";
    };

    print(left);
    print(coefficients.range);
    print(right);

    return s.str();
}
}
}
}
