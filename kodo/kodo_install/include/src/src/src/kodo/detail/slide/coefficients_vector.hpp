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
#include <cstring>
#include <memory>
#include <ostream>
#include <vector>

#include <recycle/unique_pool.hpp>

#include "../byte_index.hpp"
#include "coefficients_view.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{

/// The vector object contains the memory of a given frame or range.
///
/// Example: Consider a frame f {0,8}; i.e. it starts at offset 0 and
///          contains 8 symbols. In the binary field this would be a
///          vector object with offset 0 and 1 byte of memory to store the
///          actual data.
///
struct coefficients_vector
{
public:
    using memory_pool = recycle::unique_pool<std::vector<uint8_t>>;

public:
    /// Default constructor
    coefficients_vector() = default;

    /// Create a new vector
    /// @param lower_bound The lower_bound in the byte stream
    /// @param data The vector containing the data
    coefficients_vector(byte_index lower_bound, memory_pool::pool_ptr data) :
        m_lower_bound(lower_bound), m_data(std::move(data))
    {
        assert(m_data);
    }

    /// @param upper_bound Expand the vector to contain up until the upper_bound
    void extend(byte_index upper_bound)
    {
        assert(m_data);

        // We expect the vector to expand - so the upper_bound should be
        // increasing.
        assert(coefficients_vector::upper_bound() <= upper_bound);

        byte_count new_size = upper_bound - m_lower_bound;

        m_data->resize(new_size.value, 0U);
    }

    /// Copy the view to the beginning of the vector
    void set(const const_coefficients_view& view)
    {
        assert(lower_bound() <= view.lower_bound());
        assert(upper_bound() >= view.upper_bound());
        assert(view.bytes() > byte_count{0U});
        assert(view.data() != nullptr);
        assert(data() != nullptr);
        assert(bytes() > byte_count{0U});

        byte_count offset = view.lower_bound() - lower_bound();

        uint8_t* dst = data() + offset.value;
        const uint8_t* src = view.data();
        byte_count bytes = view.bytes();

        std::memcpy(dst, src, bytes.value);
    }

    /// Copy the data to the beginning of the vector
    void set(const uint8_t* src, byte_count bytes)
    {
        assert(src != nullptr);
        assert(bytes > byte_count{0U});

        uint8_t* dst = data();
        assert(dst != nullptr);

        std::memcpy(dst, src, bytes.value);
    }

    /// @return The data pointer of the vector
    uint8_t* data() const
    {
        assert(m_data);
        return m_data->data();
    }

    /// @return The size of the vector in bytes
    byte_count bytes() const
    {
        assert(m_data);
        return byte_count{m_data->size()};
    }

    /// @return the Upper bound of the memory
    byte_index lower_bound() const
    {
        return m_lower_bound;
    }

    /// @return the Upper bound of the memory
    byte_index upper_bound() const
    {
        assert(m_data);
        return m_lower_bound + bytes();
    }

    /// @return a const_coefficients_view to the memory of the vector
    const_coefficients_view to_const_view() const
    {
        assert(m_data);

        return const_coefficients_view{data(), lower_bound(), bytes()};
    }

    /// @return a const_coefficients_view to the memory of the vector
    mutable_coefficients_view to_mutable_view() const
    {
        assert(m_data);

        return mutable_coefficients_view{data(), lower_bound(), bytes()};
    }

    /// @return True of two memory objects are equal
    friend inline bool operator==(const coefficients_vector& lhs,
                                  const coefficients_vector& rhs)
    {
        if (lhs.m_lower_bound != rhs.m_lower_bound)
        {
            return false;
        }

        const auto& lhs_data = lhs.m_data;
        const auto& rhs_data = rhs.m_data;

        return *lhs_data == *rhs_data;
    }

    /// @param out The ostream to we will write our object representation
    /// @param v The vector to write.
    /// @return The used ostream
    friend std::ostream& operator<<(std::ostream& out,
                                    const coefficients_vector& vector)
    {
        bourne::json json;
        json["slide_coefficients_vector"]["lower_bound"] =
            vector.lower_bound().value;
        json["slide_coefficients_vector"]["upper_bound"] =
            vector.upper_bound().value;
        json["slide_coefficients_vector"]["bytes"] = vector.bytes().value;

        return out << json.dump();
    }

private:
    /// The lower bound of the vector
    byte_index m_lower_bound{0};

    /// Shared pointer containing the memory storing the coding coefficients
    memory_pool::pool_ptr m_data{nullptr};
};
}
}
}
}
