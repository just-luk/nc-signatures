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

#include <cstdint>
#include <functional>
#include <memory>

#include "../finite_field.hpp"
#include "../log_callback.hpp"
#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace block
{
/// @brief A plain RLNC symbol encoder.
///
/// This encoder does not provide a payload API, the user can directly access
/// the encoded symbol and the corresponding coding vector.
class encoder
{
public:
    /// Default constructor
    encoder();

    /// @brief The encoder constructor
    ///
    /// @param field the chosen finite field
    encoder(finite_field field);

    /// Copy constructor (disabled). This type is only movable.
    encoder(const encoder& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    encoder& operator=(const encoder& other) = delete;

    /// R-value copy constructor
    encoder(encoder&& other);

    /// R-value move assign operator
    encoder& operator=(encoder&& other);

    /// Destructor
    ~encoder();

    /// Configure the encoder with the given parameters. This is also useful for
    /// reusing an existing coder.
    ///
    /// Note that the reconfiguration always implies a reset,
    /// so the encoder will be in a clean state after this operation.
    ///
    /// @param symbols The number of symbols
    /// @param symbol_bytes The size of a symbol in bytes
    void configure(uint64_t symbols, std::size_t symbol_bytes);

    /// Reset the state of the encoder.
    void reset();

    /// @return The configured finite field.
    auto field() const -> finite_field;

    /// @return The number of symbols supported by this encoder
    auto symbols() const -> uint64_t;

    /// @return The size in bytes per symbol supported by this encoder
    auto symbol_bytes() const -> std::size_t;

    /// @return The number of bytes that can be encoded with this encoder.
    auto block_bytes() const -> std::size_t;

    /// @return The rank of the decoder
    auto rank() const -> std::size_t;

    /// Initialize all the symbols in the block.
    ///
    /// @param symbols_storage The buffer containing all the data for the block.
    void set_symbols_storage(const uint8_t* symbols_storage);

    /// Initialize a single symbol in the block.
    ///
    /// @param symbol_storage The buffer containing all the data for the the
    ///        block
    /// @param index The index of the symbol. Note the index must be equal to
    ///              encoder::rank() + 1.
    void set_symbol_storage(const uint8_t* symbol_storage, uint64_t index);

    /// Creates a new encoded symbol given the passed encoding coeffcients.
    ///
    /// @param symbol This is the memory where the encoded symbol
    ///        will be produced. This buffer must be large enough to store
    ///        symbol_bytes() bytes.
    /// @param coefficients These are the coding coefficients
    void encode_symbol(uint8_t* symbol, const uint8_t* coefficients) const;

    /// Creates a new systematic, i.e., un-coded symbol given the passed index.
    ///
    /// @param symbol This is the memory where the systeamtic symbol
    ///        will be produced. This buffer must be large enough to store
    ///        symbol_bytes() bytes.
    /// @param index The index of the systematic symbol to produce.
    void encode_systematic_symbol(uint8_t* symbol, uint64_t index) const;

    /// @return True if symbol is set, otherwise false.
    auto is_symbol_set(uint64_t index) const -> bool;

    /// Enables the log.
    /// @param callback The callback which handles the log message.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void enable_log(const log_callback& callback, void* user_data);

    /// Disables the log.
    void disable_log();

    /// @return True if log is enabled, otherwise False.
    auto is_log_enabled() const -> bool;

    /// Set a log name which will be included with log messages produced by
    /// this object.
    /// @param name The log name to set.
    void set_log_name(const std::string& name);

    /// @return The log name assigned to this object.
    auto log_name() const -> std::string;

public:
    // Implementation interface
    struct interface;

private:
    /// Pointer to the implementation
    std::unique_ptr<interface> m_impl;
};
}
}
}
