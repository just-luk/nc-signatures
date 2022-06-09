// Copyright Steinwurf ApS 2017.
// Distributed under the "STEINWURF EVALUATION LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>
#include <cstdint>
#include <initializer_list>

#include "finite_field.hpp"
#include "version.hpp"

#include "field/binary.hpp"
#include "field/binary16.hpp"
#include "field/binary4.hpp"
#include "field/binary8.hpp"
#include "field/get_value.hpp"
#include "field/prime2325.hpp"
#include "field/set_value.hpp"

#include "field/bytes_to_elements.hpp"
#include "field/elements_to_bytes.hpp"

/// The functions defined in this file provide an inline implementation for
/// these frequently used field operations. The optimizer can inline these
/// functions to improve performance.

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
/// @copydoc math::get_value()
/// @param field the selected finite field
inline uint32_t get_value(fifi::finite_field field, const uint8_t* elements,
                          std::size_t index)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return field::get_value(field::binary(), elements, index);
    case fifi::finite_field::binary4:
        return field::get_value(field::binary4(), elements, index);
    case fifi::finite_field::binary8:
        return field::get_value(field::binary8(), elements, index);
    case fifi::finite_field::binary16:
        return field::get_value(field::binary16(), elements, index);
    case fifi::finite_field::prime2325:
        return field::get_value(field::prime2325(), elements, index);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

/// @copydoc math::set_value()
/// @param field the selected finite field
inline void set_value(fifi::finite_field field, uint8_t* elements,
                      std::size_t index, uint32_t value)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        field::set_value(field::binary(), elements, index, value);
        break;
    case fifi::finite_field::binary4:
        field::set_value(field::binary4(), elements, index, value);
        break;
    case fifi::finite_field::binary8:
        field::set_value(field::binary8(), elements, index, value);
        break;
    case fifi::finite_field::binary16:
        field::set_value(field::binary16(), elements, index, value);
        break;
    case fifi::finite_field::prime2325:
        field::set_value(field::prime2325(), elements, index, value);
        break;
    default:
        assert(0 && "Unknown field");
        break;
    }
}

/// Sets a number of field values, for example:
///
///    std::vector(uint32_t> data(4);
///    fifi::set_values(fifi::finite_field::binary8, data.data(), {0, 1, 1, 0});
///
/// @param field The selected finite field
/// @param elements Pointer to the data buffer where the values should be
///        initialized
/// @param values The actual values to use
inline void set_values(fifi::finite_field field, uint8_t* elements,
                       const std::initializer_list<uint32_t> values)
{
    assert(values.size() > 0);
    std::size_t i = 0;

    for (auto v : values)
    {
        set_value(field, elements, i, v);
        ++i;
    }
}

/// @copydoc math::elements_to_bytes()
/// @param field the selected finite field
inline std::size_t elements_to_bytes(fifi::finite_field field,
                                     std::size_t elements)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return field::elements_to_bytes(field::binary(), elements);
    case fifi::finite_field::binary4:
        return field::elements_to_bytes(field::binary4(), elements);
    case fifi::finite_field::binary8:
        return field::elements_to_bytes(field::binary8(), elements);
    case fifi::finite_field::binary16:
        return field::elements_to_bytes(field::binary16(), elements);
    case fifi::finite_field::prime2325:
        return field::elements_to_bytes(field::prime2325(), elements);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

/// @copydoc math::bytes_to_elements()
/// @param field the selected finite field
inline std::size_t bytes_to_elements(fifi::finite_field field, std::size_t size)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return field::bytes_to_elements(field::binary(), size);
    case fifi::finite_field::binary4:
        return field::bytes_to_elements(field::binary4(), size);
    case fifi::finite_field::binary8:
        return field::bytes_to_elements(field::binary8(), size);
    case fifi::finite_field::binary16:
        return field::bytes_to_elements(field::binary16(), size);
    case fifi::finite_field::prime2325:
        return field::bytes_to_elements(field::prime2325(), size);
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}

/// @copydoc math::max_value()
/// @param field the selected finite field
inline std::size_t max_value(fifi::finite_field field)
{
    switch (field)
    {
    case fifi::finite_field::binary:
        return field::binary::max_value;
    case fifi::finite_field::binary4:
        return field::binary4::max_value;
    case fifi::finite_field::binary8:
        return field::binary8::max_value;
    case fifi::finite_field::binary16:
        return field::binary16::max_value;
    case fifi::finite_field::prime2325:
        return field::prime2325::max_value;
    default:
        assert(0 && "Unknown field");
        return 0U;
    }
}
}
}
