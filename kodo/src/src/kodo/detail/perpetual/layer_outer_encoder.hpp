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
#include <vector>

#include "../byte_count.hpp"
#include "../ceil_division.hpp"
#include "../reverse.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"
#include "../symbol_range.hpp"
#include "../to_symbol_range.hpp"

#include "get_bit.hpp"
#include "outer_generator.hpp"
#include "symbol_type.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace perpetual
{

/// The outer encoder - needs to create the outer code symbols
template <class Super>
struct layer_outer_encoder : public Super
{
    using inner_coefficients_type = typename Super::inner_coefficients_type;

    /// Configure the layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_coefficients.resize(Super::outer_segments());
    }

    /// Set the storage of the perpetual code - this triggers the
    /// creation of the outer code symbols
    void set_symbols_storage(const uint8_t* storage)
    {
        assert(storage != nullptr);
        Super::set_symbols_storage(storage);

        if (Super::has_outer_code())
        {
            prepare_outer_code();
        }
    }

private:
    /// Prepare outer code symbols - essentially we have to perform the outer
    /// encoding of all the outer symbols.
    void prepare_outer_code()
    {
        // We need to loop in reverse since outer symbols may include other
        // outer symbols - however if we loop from the back we ensure that
        // those outer symbols have already been initialized.
        for (outer_index index = Super::last_outer_symbol();;
             index -= symbol_count{1})
        {
            prepare_outer_symbol(index);

            // Break the loop like so as the loop is reversed and the iterator
            // is an unsigned variable.
            if (index == outer_index{0})
                break;
        }
    }

    /// Prepare outer code symbol
    void prepare_outer_symbol(outer_index outer_symbol_index)
    {
        symbol_index index = Super::to_symbol_index(outer_symbol_index);

        // The valid symbol indices that can be part of this outer symbol
        detail::symbol_range outer_symbols = Super::outer_range(index);

        // The offset of the outer symbol range
        symbol_index offset = outer_symbols.lower_bound();

        // Generate the encoding vector
        Super::generate_outer_coefficients(index, m_coefficients.data());

        // Destination of the encoded symbol
        uint8_t* symbol_data = Super::outer_symbol_data(outer_symbol_index);

        // Loop over the bits in the encoding vector and add the source symbols.
        // Again we loop from the back to ensure that if an outer symbol
        // includes any other outer symbols those will already have been
        // initialized
        for (symbol_index i : reverse(outer_symbols))
        {
            if (!get_bit(m_coefficients.data(), offset.value, i.value))
            {
                // The symbol is not included in the encoding vector
                continue;
            }

            const uint8_t* data_i = Super::symbol_data(i);
            byte_count bytes_i = Super::symbol_bytes(i);

            Super::vector_add_into(symbol_data, data_i, bytes_i);
        }
    }

private:
    /// Storage for the coefficients
    std::vector<inner_coefficients_type> m_coefficients;
};

}
}
}
}
