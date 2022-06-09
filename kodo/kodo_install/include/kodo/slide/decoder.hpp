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
#include "../symbol_pivot_callback.hpp"
#include "../version.hpp"

#include "range.hpp"
#include "reset_callback.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{

/// @brief An RLNC sliding window decoder.
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

    /// Configures the decoder.
    /// This must be called before anything else. If needed, configure can be
    /// called again. This is useful for reusing an existing coder.
    /// Note that a reconfiguration always implies a reset,
    /// so the coder will be in a clean state after the operation.
    /// @param max_symbol_bytes the maximum size of symbols in bytes. All
    /// buffers provided to the decoder (decoder::decode_symbol() and
    /// decoder::decode_systematic_symbol()) must be allocated to
    /// contain atleast this many bytes.
    void configure(std::size_t max_symbol_bytes);

    /// Resets the coder and ensures that the object is in a clean state.
    /// A coder may be reset many times.
    /// @param callback A callback that will be called once for each symbol in
    /// the decoder. If this is not needed a nullptr can be passed.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void reset(const reset_callback& callback = nullptr,
               void* user_data = nullptr);

    /// @return The finite field used.
    auto field() const -> kodo::finite_field;

    /// @return The total number of in the stream symbols.
    ///         The number of symbols in the coding window must be less than
    ///         or equal to the number of stream symbols.
    auto stream_symbols() const -> uint64_t;

    /// @return Whether the stream is empty or not.
    auto is_stream_empty() const -> bool;

    /// @return The index of the oldest symbol known by the decoder.
    auto stream_lower_bound() const -> uint64_t;

    /// @return The index of the newset symbol known by the decoder.
    auto stream_upper_bound() const -> uint64_t;

    /// @return The range of valid symbol indices. The stream is a half-open
    /// interval. Going from decoder::stream_lower_bound() to
    /// decoder::stream_upper_bound() - 1.
    auto stream_range() const -> range;

    /// @param index The index of the symbol.
    /// @return True if symbol with the given index is in the stream's range,
    /// otherwise false.
    auto in_stream(uint64_t index) const -> bool;

    /// Set the index of the oldest symbol known by the decoder.
    /// This must only be called on a decoder with an empty stream.
    /// This can be checked with decoder::is_stream_empty().
    /// @param stream_lower_bound The new stream lower bound, the new lower
    ///                           bound must be larger than or equal to the
    ///                           prior lower bound.
    void set_stream_lower_bound(uint64_t stream_lower_bound);

    /// Push a symbol on to the decoder's FIFO stream. This prepares the decoder
    /// for a new symbol. Initially the symbol is uninitialized, but may or may
    /// not be initialized at a later point when calling
    /// decoder::decode_symbol() or decoder::decode_systematic_symbol().
    void push_symbol();

    /// Pop the oldest symbol from the decoder's FIFO stream. This
    /// increments decoder::stream_lower_bound().
    /// @return The pointer to the popped symbol. If the symbol was not
    /// initialized, a nullptr is returned. Otherwise it is the responsability
    /// of the caller to free or reuse the memory.
    auto pop_symbol() -> uint8_t*;

    /// Decode a coded symbol according to the given coding coefficients and
    /// window.
    ///
    /// .. note:: The caller must ensure that the memory of the symbol remains
    ///           valid as long as the symbol is included in the stream.
    ///
    /// .. note:: The symbol buffer must be allocated so that it can contain
    ///           atleast decoder::max_symbol_bytes() bytes, as these bytes may
    ///           be used when decoding the symbols.
    ///
    /// @param symbol Pointer to the symbol.
    /// @param symbol_bytes the size of the given symbol in bytes. This must be
    ///                     less than or equal to the maximum number of bytes
    /// @param window The window for the coded symbol. The window lower/upper
    ///        bound must be contained within the decoder's stream (see
    ///        decoder::stream_lower_bound() and decoder::stream_upper_bound()).
    /// @param coefficients The coding coefficients used to create the coded
    ///                     symbol.
    /// @return nullptr if the symbol was "claimed" by the decoder, i.e., the
    ///         symbol was linear independent.
    ///         Otherwise a data pointer to the given symbol or a different, but
    ///         no longer needed, symbol will be returned.
    ///         In cases where a non null pointer is returned it is the
    ///         responsability of the caller to free or reuse the memory.
    auto decode_symbol(uint8_t* symbol, std::size_t symbol_bytes, range window,
                       uint8_t* coefficients) -> uint8_t*;

    /// Add a systematic symbol to the stream.
    ///
    /// .. note:: The caller must ensure that the memory of the symbol remains
    ///           valid as long as the symbol is included in the stream.
    ///
    /// .. note:: The symbol buffer must be allocated so that it can contain
    ///           atleast decoder::max_symbol_bytes() bytes, as these bytes may
    ///           be used when decoding the symbols.
    ///
    /// @param symbol Pointer to the symbol.
    /// @param symbol_bytes the size of the given symbol in bytes. This must be
    ///                     less than or equal to the maximum number of bytes
    /// @param index The position of the symbol in the stream. The index must be
    /// contained within the decoder's stream (see decoder::stream_lower_bound()
    /// and decoder::stream_upper_bound()).
    ///
    /// @return nullptr if the symbol was "claimed" by the decoder, i.e., the
    ///         symbol was linear independent.
    ///         Otherwise a pointer to the given or a different, but no longer
    ///         needed, symbol will be returned.
    ///         In cases where a non null pointer is returned it is the
    ///         responsability of the caller to free or reuse the memory.
    auto decode_systematic_symbol(uint8_t* symbol, std::size_t symbol_bytes,
                                  uint64_t index) -> uint8_t*;

    /// The rank of a decoder indicates how many symbols have been partially or
    /// fully decoded. This number is equivalent to the number of pivot elements
    /// in the stream.
    ///
    /// @return The rank of the decoder
    auto rank() const -> uint64_t;

    /// @return The number of missing symbols in the stream.
    auto symbols_missing() const -> uint64_t;

    /// @return The number of partially decoded symbols in the stream.
    auto symbols_partially_decoded() const -> uint64_t;

    /// @return The number of decoded symbols in the stream.
    auto symbols_decoded() const -> uint64_t;

    /// @param index The index of the symbol. Index must be between
    ///              decoder::stream_lower_bound() and
    ///              decoder::stream_upper_bound().
    ///
    /// @return the pointer associated with a specific symbol
    auto symbol_data(uint64_t index) const -> const uint8_t*;

    /// @param index The index of the symbol. Index must be between
    ///              decoder::stream_lower_bound() and
    ///              decoder::stream_upper_bound().
    ///
    /// @return the pointer associated with a specific symbol
    auto symbol_data(uint64_t index) -> uint8_t*;

    /// Return a "guestimate" of the symbol size at the given index in the
    /// decoder.
    /// @param index The index of the symbol.
    /// @return The size of the symbol at the given index in the decoder. This
    /// size is a "guestimate" of the actual size. It will never be less than
    /// the actual size of the symbol, but depending on the decoding process it
    /// may be larger.
    auto symbol_bytes(uint64_t index) const -> std::size_t;

    /// @return The maximum number of bytes per symbol.
    auto max_symbol_bytes() const -> std::size_t;

    /// @param index Index of the symbol to check.
    ///
    /// @return True if the symbol is decoded (i.e. it corresponds to a source
    ///         symbol), and otherwise false.
    auto is_symbol_decoded(uint64_t index) const -> bool;

    /// @param index Index of the symbol to check
    ///
    /// @return True if the symbol is pivot. Pivot means that the symbol is
    /// either fully decoded or partially decoded.
    ///
    auto is_symbol_pivot(uint64_t index) const -> bool;

    /// Register a custom callback that will be invoked every time a symbol is
    /// decoded
    /// @param callback The callback which handles when a symbol is decoded.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void on_symbol_decoded(const symbol_decoded_callback& callback,
                           void* user_data);

    /// Register a custom callback that will be invoked every time a symbol is
    /// found to be a pivot
    /// @param callback The callback which handles when a symbol is set to be
    /// pivot.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void on_symbol_pivot(const symbol_pivot_callback& callback,
                         void* user_data);

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
