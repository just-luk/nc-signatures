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

#include <fifi/field/bytes_to_elements.hpp>
#include <fifi/field/elements_to_bytes.hpp>
#include <fifi/field/get_value.hpp>
#include <fifi/field/set_value.hpp>

#include "../finite_field.hpp"
#include "../version.hpp"

#include "byte_count.hpp"

#include "symbol_count.hpp"
#include "symbol_index.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// @brief Basic layer performing common finite field operations
template <class Math, class Super>
class layer_math : public Super
{
public:
    /// The field type we use
    using field_type = typename Math::field_type;

public:
    auto math() -> Math&
    {
        return m_math;
    }

    auto math() const -> const Math&
    {
        return m_math;
    }

    auto field() const -> finite_field
    {
        switch (m_math.field())
        {
        case fifi::finite_field::binary:
            return finite_field::binary;
        case fifi::finite_field::binary4:
            return finite_field::binary4;
        case fifi::finite_field::binary8:
            return finite_field::binary8;
        case fifi::finite_field::binary16:
            return finite_field::binary16;
        default:
            assert(0 && "Unknown field");
            return finite_field::binary;
        }
    }

    auto bytes_to_elements(byte_count bytes) const -> symbol_count
    {
        return symbol_count{
            fifi::field::bytes_to_elements(field_type{}, bytes.value)};
    }

    auto elements_to_bytes(symbol_count elements) const -> byte_count
    {
        return byte_count{
            fifi::field::elements_to_bytes(field_type{}, elements.value)};
    }

    void set_value(uint8_t* data, symbol_index index, uint32_t value) const
    {
        assert(data != nullptr);

        fifi::field::set_value(field_type{}, data, index.value, value);
    }

    auto get_value(const uint8_t* data, symbol_index index) const -> uint32_t
    {
        assert(data != nullptr);

        return fifi::field::get_value(field_type{}, data, index.value);
    }

    auto field_info() const -> field_type
    {
        return m_math.field_info();
    }

    auto is_binary() const -> bool
    {
        return m_math.field() == fifi::finite_field::binary;
    }

    auto divide(uint32_t a, uint32_t b) const -> uint32_t
    {
        return m_math.divide(a, b);
    }

    void vector_add_into(uint8_t* x, const uint8_t* y, byte_count bytes) const
    {
        assert(x != nullptr);
        assert(y != nullptr);
        assert(bytes > byte_count{0});

        m_math.vector_add_into(x, y, bytes.value);
    }

    void vector_subtract_into(uint8_t* x, const uint8_t* y,
                              byte_count bytes) const
    {
        assert(x != nullptr);
        assert(y != nullptr);
        assert(bytes > byte_count{0});

        m_math.vector_subtract_into(x, y, bytes.value);
    }

    auto vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                       uint32_t constant,
                                       byte_count bytes) const
    {
        assert(x != nullptr);
        assert(y != nullptr);
        assert(bytes > byte_count{0});

        m_math.vector_multiply_subtract_into(x, y, constant, bytes.value);
    }

    void vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                  uint32_t constant, byte_count bytes) const
    {
        assert(x != nullptr);
        assert(y != nullptr);
        assert(bytes > byte_count{0});

        m_math.vector_multiply_add_into(x, y, constant, bytes.value);
    }

    auto invert(uint32_t value) const -> uint32_t
    {
        return m_math.invert(value);
    }

    void vector_multiply_into(uint8_t* x, uint32_t constant,
                              byte_count bytes) const
    {
        assert(x != nullptr);
        assert(bytes > byte_count{0});

        m_math.vector_multiply_into(x, constant, bytes.value);
    }

private:
    /// The selected field
    Math m_math;
};
}
}
}
