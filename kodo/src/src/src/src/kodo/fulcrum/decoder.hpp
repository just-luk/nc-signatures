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
#include "../symbol_decoded_callback.hpp"
#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace fulcrum
{
/// @brief A plain RLNC symbol decoder.
///
/// This decoder does not provide a payload API, the user can directly access
/// the encoded symbol and the corresponding coding vector.
class decoder
{
public:
    /// Default constructor
    decoder();

    /// @brief The decoder constructor
    ///
    /// @param field the chosen finite field
    decoder(finite_field field);

    /// Copy constructor (disabled). This type is only movable.
    decoder(const decoder& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    decoder& operator=(const decoder& other) = delete;

    /// R-value copy constructor
    decoder(decoder&& other);

    /// R-value move assign operator
    decoder& operator=(decoder&& other);

    /// Destructor
    ~decoder();

    /// Configure the decoder with the given parameters. This is also useful for
    /// reusing an existing coder.
    ///
    /// Note that the reconfiguration always implies a reset,
    /// so the decoder will be in a clean state after this operation.
    ///
    /// @param symbols The number of symbols
    /// @param symbol_bytes The size of a symbol in bytes
    /// @param expansion The number of expansion symbols to use
    void configure(std::size_t symbols, std::size_t symbol_bytes,
                   std::size_t expansion);

    /// Reset the state of the decoder.
    void reset();

    /// @return The configured finite field.
    auto field() const -> finite_field;

    /// @return The inner finite field.
    auto inner_field() const -> finite_field;

    /// @return The number of symbols supported by this decoder
    auto symbols() const -> uint64_t;

    /// @return The expansion set for this decoder
    auto expansion() const -> uint64_t;

    /// @return The number of inner symbols
    auto inner_symbols() const -> uint64_t;

    /// @return The size in bytes per symbol supported by this decoder
    auto symbol_bytes() const -> std::size_t;

    /// @return the total number of bytes that can be decoded with this decoder.
    auto block_bytes() const -> std::size_t;

    /// Initialize a symbols in the block
    ///
    /// @param symbol_storage The buffer containing all the data for the symbol
    /// @param index The index of the symbol.
    void set_symbol_storage(uint8_t* symbol_storage, uint64_t index);

    /// Initialize all the symbols in the block
    ///
    /// @param symbols_storage The buffer containing all the data for the the
    ///        block
    void set_symbols_storage(uint8_t* symbols_storage);

    /// @return The memory for the symbol.
    /// @param index The index of the symbol.
    auto symbol_data(uint64_t index) const -> const uint8_t*;

    /// Feed a coded symbol to the decoder.
    ///
    /// @param symbol The data of the symbol assumed to be symbol_bytes()
    ///        bytes in size.
    /// @param coefficients The coding coefficients that describe the encoding
    ///        performed on the symbol.
    void decode_symbol(uint8_t* symbol, uint8_t* coefficients);

    /// Feed a systematic, i.e., un-coded symbol to the decoder.
    ///
    /// @param symbol The data of the symbol assumed to be symbol_bytes()
    ///        bytes in size.
    /// @param index The index of the given symbol.
    void decode_systematic_symbol(const uint8_t* symbol, uint64_t index);

    /// Recodes a new encoded symbol given the passed encoding coeffcients.
    /// Coefficient values for symbols not seen must be 0.
    /// This can be ensured, e.g., by using the random_uniform generator's
    /// generator::random_uniform::generate_recode() function.
    ///
    /// @param symbol This is the memory where the encoded symbol
    ///        will be produced. This buffer must be large enough to store
    ///        symbol_bytes() bytes.
    /// @param coefficients These are the coding coefficients
    /// @param coefficients_in These are the input coding coefficients
    void recode_symbol(uint8_t* symbol, uint8_t* coefficients,
                       const uint8_t* coefficients_in) const;

    /// @return The rank of the decoder
    auto rank() const -> uint64_t;

    /// @return The rank of the inner decoder
    auto inner_rank() const -> uint64_t;

    /// @return True if the decoder contains a pivot at the specific index.
    auto is_symbol_pivot(uint64_t index) const -> bool;

    /// @return True if the decoder contains a pivot at the specific index.
    auto is_inner_symbol_pivot(uint64_t index) const -> bool;

    /// @return True if the decoder contains a decoded symbol at the specific
    /// index.
    auto is_symbol_decoded(uint64_t index) const -> bool;

    /// Register a custom callback that will be invoked every time a symbol is
    /// decoded
    /// @param callback The callback which handles when a symbol is decoded.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void on_symbol_decoded(const symbol_decoded_callback& callback,
                           void* user_data);

    /// @return True if the decoder is complete, when this is true the content
    ///         stored in symbols_storage is decoded.
    auto is_complete() const -> bool;

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
