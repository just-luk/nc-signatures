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

#include <cassert>

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace detail
{
/// Simple container for a generic pointer. When initialized it can either be
/// enabled or disabled. When disabled the pointer is not allowed to be
/// accessed.
template <class T>
class enabled_ptr
{
public:
    /// Constructor
    enabled_ptr() : m_t(nullptr), m_is_enabled(false)
    {
    }

    /// Constructs an enabled pointer
    enabled_ptr(const T* t, bool is_enabled) : m_t(t), m_is_enabled(is_enabled)
    {
        assert(m_t != nullptr);

        if (is_enabled)
        {
            // Check that if the CPU supports the acceleration we also have
            // compiled support for it.
            assert(t->is_compiled());
        }
    }

    /// Access the internal pointer.
    /// Make sure is_enabled returns true before using this functionality.
    const T* operator->() const
    {
        assert(m_is_enabled);
        return m_t;
    }

    /// Get a reference to the internal object.
    /// Make sure is_enabled returns true before using this functionality.
    const T& get() const
    {
        assert(m_is_enabled);
        return *m_t;
    }

    /// @return True if the pointer is enabled, otherwise false.
    bool is_enabled() const
    {
        return m_is_enabled;
    }

private:
    const T* m_t;
    bool m_is_enabled;
};
}
}
}
