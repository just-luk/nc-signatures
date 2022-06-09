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

#include <random>
#include <tuple>

namespace helper
{
/// Helper function that allows a bunch of permutations of parameters
/// to be created:
///
/// Example:
///
///    std::vector<std::size_t> a {1, 2, 3};
///    std::vector<char> b {'a', 'b', 'c'};
///
///    combine(
///        [](auto v){
///            std::cout << std::get<0>(v) << "\n";
///            std::cout << std::get<1>(v) << "\n";
///        }, a, b);
///
/// Here we invoked the lambda with all permutations of values from a and b
///
namespace detail
{
template <class Function, class... Values>
inline void combine(Function function, std::tuple<Values...> elements)
{
    function(elements);
}

template <class Function, class Range, class... Values>
inline void combine(Function function, std::tuple<Values...> elements,
                    Range range)
{
    for (auto element : range)
    {
        function(std::tuple_cat(elements, std::make_tuple(element)));
    }
}

template <class Function, class Range, class... Values, class... Ranges>
inline void combine(Function function, std::tuple<Values...> elements,
                    Range range, Ranges... ranges)
{
    for (auto element : range)
    {
        combine(function, std::tuple_cat(elements, std::make_tuple(element)),
                ranges...);
    }
}
}

template <class Function, class Range, class... Ranges>
inline void combine(Function function, Range range, Ranges... ranges)
{
    for (auto element : range)
    {
        detail::combine(function, std::make_tuple(element), ranges...);
    }
}

}