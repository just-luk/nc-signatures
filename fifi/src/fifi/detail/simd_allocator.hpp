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

#include "../version.hpp"

#include <cstddef>
#include <cstdint>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{

/// Provides the non typed allocator functions
struct aligned_allocator
{
    void* allocate(std::size_t bytes, std::size_t alignment);
    void deallocate(void* ptr, std::size_t bytes);
};

/// Using the allocator boilerplate code provided by Howard Hinnant here:
/// https://howardhinnant.github.io/allocator_boilerplate.html
///
/// Problem with alignment in pre C++17:
/// https://gcc.gnu.org/bugzilla//show_bug.cgi?id=65122
///
/// After C++17 we can drop this allocator - I think.
template <class T>
class simd_allocator : aligned_allocator
{
public:
    using value_type = T;

    simd_allocator()
    {
    }

    template <class U>
    simd_allocator(simd_allocator<U> const&)
    {
    }

    value_type* allocate(std::size_t n)
    {
        return static_cast<value_type*>(
            aligned_allocator::allocate(n * sizeof(T), alignof(T)));
    }

    void deallocate(value_type* p, std::size_t size)
    {
        aligned_allocator::deallocate(p, size);
    }
};

template <class T, class U>
bool operator==(simd_allocator<T> const&, simd_allocator<U> const&)
{
    return true;
}

template <class T, class U>
bool operator!=(simd_allocator<T> const& x, simd_allocator<U> const& y)
{
    return !(x == y);
}

}
}
}
