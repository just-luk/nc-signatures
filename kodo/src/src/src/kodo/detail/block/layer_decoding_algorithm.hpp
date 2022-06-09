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

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../optional_index.hpp"
#include "../symbol_count.hpp"
#include "../symbol_index.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace block
{
/// @brief Implements basic linear block decoder.
///
/// The linear block decoder expects that an encoded symbol is described by
/// a vector of coefficients. Using these coefficients, the block decoder
/// subtracts incoming symbols until the original data is recreated.
///
/// The linear block decoder will search for pivots from the left of the
/// encoding vector. E.g. if you see an encoding vector like:
///
///   +-----------> Direction of search for pivots
///
///   0 1 0 1 1 0 0
///   ^ ^         ^
///   | |         |
///   | |         +----+  Last coefficient searched
///   | +--------------+  First non zero coefficient
///   +----------------+  First coefficient searched
template <class Super>
class layer_decoding_algorithm : public Super
{

public:
    /// Configure this layer
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        m_maximum_pivot = symbol_index{0};
    }

    void decode_symbol(uint8_t* symbol_data, uint8_t* coefficients)
    {

        assert(symbol_data != nullptr);
        assert(coefficients != nullptr);
        assert(Super::symbols_set() == Super::symbols() &&
               "Define the symbol storage for the decoder before "
               "reading a symbol");

        decode_coefficients(symbol_data, coefficients);

        if (is_complete())
            Super::set_symbols_decoded();
    }

    void decode_systematic_symbol(const uint8_t* symbol_data,
                                  symbol_index index)
    {

        assert(index <= Super::last_symbol());
        assert(symbol_data != nullptr);
        assert(Super::symbols_set() == Super::symbols() &&
               "Define the symbol storage for the decoder before "
               "reading a symbol");

        if (Super::is_symbol_decoded(index))
        {
            return;
        }

        if (Super::is_symbol_partially_decoded(index))
        {
            swap_decode(symbol_data, index);
        }
        else
        {
            // Stores the symbol and updates the corresponding
            // encoding vector
            store_uncoded_symbol(symbol_data, index);

            // Backwards substitution
            uint8_t* coefficients = Super::coefficients_data(index);
            backward_substitute(symbol_data, coefficients, index);

            m_maximum_pivot = std::max(index, m_maximum_pivot);
        }

        if (is_complete())
            Super::set_symbols_decoded();
    }

    bool is_complete() const
    {
        return Super::rank() == Super::symbols();
    }

protected:
    /// Decodes a symbol based on the coefficients
    /// @param symbol_data buffer containing the encoding symbol
    /// @param coefficients buffer containing the encoding vector
    void decode_coefficients(uint8_t* symbol_data, uint8_t* coefficients)
    {

        assert(symbol_data != nullptr);
        assert(coefficients != nullptr);

        // See if we can find a pivot
        optional_index pivot =
            forward_substitute_to_pivot(symbol_data, coefficients);

        if (!pivot.found())
        {
            return;
        }

        if (!Super::is_binary())
        {
            // Normalize symbol and vector
            normalize(symbol_data, coefficients, pivot.index());
        }

        // Reduce the symbol further
        forward_substitute_from_pivot(symbol_data, coefficients, pivot.index());

        // Now with the found pivot reduce the existing symbols
        backward_substitute(symbol_data, coefficients, pivot.index());

        // Now save the received symbol
        store_coded_symbol(symbol_data, coefficients, pivot.index());

        m_maximum_pivot = std::max(pivot.index(), m_maximum_pivot);
    }

    /// When adding a raw symbol (i.e. decoded) with a specific
    /// pivot id and the decoder already contains a coded symbol
    /// in that position this function performs the proper swap
    /// between the two symbols.
    /// @param symbol_data the data for the raw symbol
    /// @param pivot_index the pivot position of the raw symbol
    void swap_decode(const uint8_t* symbol_data, symbol_index pivot_index)
    {

        assert(Super::is_symbol_partially_decoded(pivot_index));
        assert(!Super::is_symbol_decoded(pivot_index));
        assert(!Super::is_symbol_missing(pivot_index));

        Super::set_symbol_missing(pivot_index);

        auto symbol_i = Super::symbol_data(pivot_index);
        auto coefficients_i = Super::coefficients_data(pivot_index);

        assert(Super::get_value(coefficients_i, pivot_index) == 1);

        // Subtract the new pivot symbol
        Super::set_value(coefficients_i, pivot_index, 0U);

        Super::vector_subtract_into(symbol_i, symbol_data,
                                    Super::symbol_bytes());

        // Continue to process our new coded symbol: we know that it must
        // contain a larger pivot id than the current (unless it is reduced
        // to all zeroes).
        decode_coefficients(symbol_i, coefficients_i);

        // The previous vector may still be in memory
        std::fill_n(coefficients_i, Super::max_coefficients_bytes().value, 0);

        // Stores the symbol and sets the pivot in the vector
        store_uncoded_symbol(symbol_data, pivot_index);

        // No need to backwards substitute since we are replacing an existing
        // symbol, i.e., backwards substitution must already have been done.
    }

    /// Normalize the encoding vector by dividing all elements in
    /// the vector with the value at the provided index
    /// @param symbol_data the data of the encoded symbol
    /// @param coefficients the data constituting the encoding vector
    /// @param index the index of the found pivot element
    void normalize(uint8_t* symbol_data, uint8_t* coefficients,
                   symbol_index index)
    {
        assert(symbol_data != nullptr);
        assert(!Super::is_binary());
        assert(coefficients != nullptr);
        assert(index <= Super::last_symbol());

        auto coefficient = Super::get_value(coefficients, index);
        assert(coefficient > 0);

        auto inverted_coefficient = Super::invert(coefficient);

        // Update symbol and corresponding vector
        Super::vector_multiply_into(coefficients, inverted_coefficient,
                                    Super::max_coefficients_bytes());

        Super::vector_multiply_into(symbol_data, inverted_coefficient,
                                    Super::symbol_bytes());
    }

    /// Iterates the encoding vector and subtracts existing symbols
    /// until a pivot element is found.
    /// @param symbol_data the data of the encoded symbol
    /// @param coefficients the data constituting the encoding vector
    /// @return the pivot index if found.
    optional_index forward_substitute_to_pivot(uint8_t* symbol_data,
                                               uint8_t* coefficients)
    {

        assert(coefficients != nullptr);
        assert(symbol_data != nullptr);

        for (symbol_index index : Super::symbol_range())
        {
            auto coefficient = Super::get_value(coefficients, index);

            if (!coefficient)
            {
                // The coefficient is zero
                continue;
            }

            if (!Super::is_symbol_pivot(index))
            {
                return optional_index{true, index};
            }

            const uint8_t* coefficients_i = Super::coefficients_data(index);
            const uint8_t* symbol_i = Super::symbol_data(index);

            if (Super::is_binary())
            {
                Super::vector_subtract_into(coefficients, coefficients_i,
                                            Super::max_coefficients_bytes());

                Super::vector_subtract_into(symbol_data, symbol_i,
                                            Super::symbol_bytes());
            }
            else
            {
                Super::vector_multiply_subtract_into(
                    coefficients, coefficients_i, coefficient,
                    Super::max_coefficients_bytes());

                Super::vector_multiply_subtract_into(
                    symbol_data, symbol_i, coefficient, Super::symbol_bytes());
            }
        }

        return optional_index::not_found();
    }

    /// Iterates the encoding vector from where a pivot has been
    /// identified and subtracts existing symbols
    /// @param symbol_data the data of the encoded symbol
    /// @param coefficients the data constituting the encoding vector
    /// @param pivot_index the index of the found pivot element
    void forward_substitute_from_pivot(uint8_t* symbol_data,
                                       uint8_t* coefficients,
                                       symbol_index pivot_index)
    {

        assert(coefficients != nullptr);
        assert(symbol_data != nullptr);
        assert(pivot_index <= Super::last_symbol());
        assert(!Super::is_symbol_partially_decoded(pivot_index));
        assert(!Super::is_symbol_decoded(pivot_index));
        assert(Super::is_symbol_missing(pivot_index));

        // If this pivot index was smaller than the maximum pivot
        // index we have, we might also need to backward
        // substitute the higher pivot values into the new packet

        // Start right after the pivot_index position
        for (std::size_t i = pivot_index.value + 1; i < Super::symbols().value;
             ++i)
        {
            symbol_index index{i};
            // Do we have a non-zero value here?
            auto coefficient = Super::get_value(coefficients, index);

            if (!coefficient)
            {
                // The coefficient is zero
                continue;
            }

            if (!Super::is_symbol_pivot(index))
            {
                // We do not have a pivot here
                continue;
            }
            auto coefficients_i = Super::coefficients_data(index);
            auto symbol_i = Super::symbol_data(index);

            if (Super::is_binary())
            {
                Super::vector_subtract_into(coefficients, coefficients_i,
                                            Super::max_coefficients_bytes());

                Super::vector_subtract_into(symbol_data, symbol_i,
                                            Super::symbol_bytes());
            }
            else
            {
                Super::vector_multiply_subtract_into(
                    coefficients, coefficients_i, coefficient,
                    Super::max_coefficients_bytes());

                Super::vector_multiply_subtract_into(
                    symbol_data, symbol_i, coefficient, Super::symbol_bytes());
            }
        }
    }

    /// Backward substitute the found symbol into the
    /// existing symbols.
    /// @param symbol_data buffer containing the encoding symbol
    /// @param coefficients buffer containing the encoding vector
    /// @param pivot_index the pivot index of the symbol in the
    ///        buffers coefficients and symbol_data
    void backward_substitute(const uint8_t* symbol_data,
                             const uint8_t* coefficients,
                             symbol_index pivot_index)
    {

        assert(coefficients != nullptr);
        assert(symbol_data != nullptr);
        assert(pivot_index <= Super::last_symbol());

        // We found a "1" that nobody else had as pivot, we now
        // substract this packet from other coded packets
        // - if they have a "1" at our pivot position
        for (std::size_t i = 0; i <= m_maximum_pivot.value; ++i)
        {
            symbol_index index{i};
            if (index == pivot_index)
            {
                // We cannot backward substitute into our self
                continue;
            }

            if (Super::is_symbol_decoded(index))
            {
                // We know that we have no non-zero elements
                // outside the pivot position.
                continue;
            }

            if (Super::is_symbol_missing(index))
            {
                // We do not have a symbol yet here
                continue;
            }

            // The symbol must be partially decoded
            assert(Super::is_symbol_partially_decoded(index));

            uint8_t* coefficients_i = Super::coefficients_data(index);
            auto coefficient = Super::get_value(coefficients_i, pivot_index);

            if (!coefficient)
            {
                // The coefficient is zero
                continue;
            }

            uint8_t* symbol_i = Super::symbol_data(index);

            if (Super::is_binary())
            {
                Super::vector_subtract_into(coefficients_i, coefficients,
                                            Super::max_coefficients_bytes());

                Super::vector_subtract_into(symbol_i, symbol_data,
                                            Super::symbol_bytes());
            }
            else
            {
                // Update symbol and corresponding vector
                Super::vector_multiply_subtract_into(
                    coefficients_i, coefficients, coefficient,
                    Super::max_coefficients_bytes());

                Super::vector_multiply_subtract_into(
                    symbol_i, symbol_data, coefficient, Super::symbol_bytes());
            }
        }
    }

    /// Store an encoded symbol and encoding vector with the specified
    /// pivot found.
    /// @param symbol_data buffer containing the encoding symbol
    /// @param coefficients buffer containing the symbol coefficients
    /// @param pivot_index the pivot index
    void store_coded_symbol(const uint8_t* symbol_data,
                            const uint8_t* coefficients,
                            symbol_index pivot_index)
    {

        assert(!Super::is_symbol_partially_decoded(pivot_index));
        assert(!Super::is_symbol_decoded(pivot_index));
        assert(Super::is_symbol_missing(pivot_index));

        assert(coefficients != nullptr);
        assert(symbol_data != nullptr);

        Super::copy_into_coefficients(pivot_index, coefficients);

        // Mark this symbol as partially decoded
        Super::set_symbol_partially_decoded(pivot_index);

        // Copy it into the symbol storage
        Super::copy_into_symbol(pivot_index, symbol_data);
    }

    /// Stores an decoded or fully decoded symbol
    /// @param symbol_data the data for the symbol
    /// @param pivot_index the pivot index of the symbol
    void store_uncoded_symbol(const uint8_t* symbol_data,
                              symbol_index pivot_index)
    {

        assert(!Super::is_symbol_partially_decoded(pivot_index));
        assert(!Super::is_symbol_decoded(pivot_index));
        assert(Super::is_symbol_missing(pivot_index));

        assert(symbol_data != nullptr);
        assert(pivot_index <= Super::last_symbol());

        // Update the corresponding vector
        uint8_t* coefficients_dest = Super::coefficients_data(pivot_index);

        // Zero out the memory first
        std::fill_n(coefficients_dest, Super::max_coefficients_bytes().value,
                    0);

        Super::set_value(coefficients_dest, pivot_index, 1U);

        // Mark this symbol decoded
        Super::set_symbol_decoded(pivot_index);

        Super::copy_into_symbol(pivot_index, symbol_data);
    }

protected:
    /// Stores the current maximum pivot index
    symbol_index m_maximum_pivot{0};
};
}
}
}
}
