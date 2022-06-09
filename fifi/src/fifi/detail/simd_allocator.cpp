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

#include "simd_allocator.hpp"
#include "../version.hpp"

#include <cassert>
#include <platform/config.hpp>

#if defined(PLATFORM_GCC_COMPATIBLE_X86)
#include <mm_malloc.h>
#elif defined(PLATFORM_MSVC_X86)
#include <malloc.h>
#endif

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{

#if defined(PLATFORM_X86)
void* aligned_allocator::allocate(std::size_t bytes, std::size_t alignment)
{
    return _mm_malloc(bytes, alignment);
}
void aligned_allocator::deallocate(void* ptr, std::size_t bytes)
{
    (void)bytes;

    _mm_free(ptr);
}

#else

void* aligned_allocator::allocate(std::size_t bytes, std::size_t alignment)
{
    (void)bytes;
    (void)alignment;

    assert(0 && "Target platform does not support aligned allocator");

    return (void*)0x0;
}
void aligned_allocator::deallocate(void* ptr, std::size_t bytes)
{
    (void)ptr;
    (void)bytes;

    assert(0 && "Target platform does not support aligned allocator");
}
#endif

}
}
}