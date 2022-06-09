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

#include <cassert>
#include <cstdint>
#include <ostream>

#include "../byte_count.hpp"
#include "../byte_index.hpp"
#include "../byte_range.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{

/// The view object contains information about the memory location of
/// range or frame objects including a pointer to that memory.
///
/// Example: Consider a frame f {0,8}; i.e. it starts at offset 0 and
///          contains 8 symbols. In the binary field this would be a
///          memory object with offset 0 and size 1. When actual memory
///          for that object is allocated we can create a view to it.
///
template <class PointerType>
struct coefficients_view
{
    /// Construct a new view
    coefficients_view(PointerType data, byte_index lower_bound,
                      byte_count size) :
        m_data(data),
        m_lower_bound(lower_bound), m_bytes(size)
    {
        assert(data != nullptr);
        assert(m_bytes > byte_count{0U});
    }

    /// Enable copy construction from a different coefficients_view an implicit
    /// pointer conversion i.e. uint8_t* -> const uint8_t*
    template <class T>
    coefficients_view(coefficients_view<T> other) :
        coefficients_view(other.data(), other.lower_bound(), other.bytes())
    {
    }

    /// @return The size of the memory in bytes
    byte_count bytes() const
    {
        return m_bytes;
    }

    /// @return the Upper bound of the memory
    byte_index lower_bound() const
    {
        return m_lower_bound;
    }

    /// @return the Upper bound of the memory
    byte_index upper_bound() const
    {
        return m_lower_bound + m_bytes;
    }

    byte_range range() const
    {
        return byte_range(m_lower_bound, upper_bound());
    }

    /// @return The data pointer of the view
    PointerType data() const
    {
        assert(m_data != nullptr);
        return m_data;
    }

private:
    /// Pointer to the data of the view
    PointerType m_data;

    /// The offset of the first symbol in bytes
    byte_index m_lower_bound{0U};

    /// The size of the memory in bytes.
    byte_count m_bytes{0U};
};

using mutable_coefficients_view = coefficients_view<uint8_t*>;
using const_coefficients_view = coefficients_view<const uint8_t*>;

/// @return True of two memory objects are equal
template <class T, class U>
inline bool operator==(const coefficients_view<T>& lhs,
                       const coefficients_view<U>& rhs)
{
    return (lhs.data() == rhs.data()) &&
           (lhs.lower_bound() == rhs.lower_bound()) &&
           (lhs.bytes() == rhs.bytes());
}

/// @return True of two memory objects are equal
template <class T, class U>
inline bool operator!=(const coefficients_view<T>& lhs,
                       const coefficients_view<U>& rhs)
{
    return !(lhs == rhs);
}

/// @param out The ostream to we will write our object representation
/// @param view The view to write.
/// @return The used ostream
template <class T>
inline std::ostream& operator<<(std::ostream& out,
                                const coefficients_view<T>& view)
{
    bourne::json json;
    json["coefficients_view"]["lower_bound"] = view.lower_bound().value;
    json["coefficients_view"]["upper_bound"] = view.upper_bound().value;
    json["coefficients_view"]["bytes"] = view.bytes().value;

    return out << json.dump();
}
}
}
}
}
