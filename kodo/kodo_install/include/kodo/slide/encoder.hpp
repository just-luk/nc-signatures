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

#include "range.hpp"
#include "reset_callback.hpp"

#include "../finite_field.hpp"
#include "../log_callback.hpp"
#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{

/// @brief An RLNC sliding window encoder.
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

    /// Configure the encoder. This must be called before anything else. If
    /// needed, configure can be called again. This is useful for reusing an
    /// existing coder. Note that the a reconfiguration always implies a reset,
    /// so the coder will be in a clean state after the operation.
    /// @param max_symbol_bytes the maximum size of symbols in bytes.
    void configure(std::size_t max_symbol_bytes);

    /// Resets the coder and ensures that the object is in a clean state.
    /// A coder may be reset many times.
    /// @param callback A callback that will be called once for each symbol in
    /// the encoder. If this is not needed a nullptr can be passed.
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

    /// @return The index of the oldest symbol known by the encoder.
    auto stream_lower_bound() const -> uint64_t;

    /// @return The index of the newest symbol known by the encoder.
    auto stream_upper_bound() const -> uint64_t;

    /// @return The range of valid symbol indices. The stream is a half-open
    /// interval. Going from encoder::stream_lower_bound() to
    /// encoder::stream_upper_bound() - 1.
    auto stream_range() const -> range;

    /// @param index The index of the symbol.
    /// @return True if symbol with the given index is in the stream's range,
    /// otherwise false.
    auto in_stream(uint64_t index) const -> bool;

    /// Push a symbol on to the encoder's FIFO stream. This increments the
    /// number of symbols in the stream, and increase the
    /// encoder::stream_upper_bound().
    ///
    /// .. note:: The caller must ensure that the memory of the symbol remains
    ///           valid as long as the symbol is included in the stream. The
    ///           caller is also responsible for freeing the memory if needed,
    ///           once the symbol is popped from the stream with
    ///           encoder::pop_symbol().
    ///
    /// @param symbol Pointer to the symbol.
    /// @param symbol_bytes the size of the symbol in bytes. This must be less
    ///                     than or equal to the maximum number of bytes per
    ///                     symbol configured with encoder::configure().
    void push_symbol(const uint8_t* symbol, std::size_t symbol_bytes);

    /// Pop the oldest symbol from the decoder's FIFO stream. This increment the
    /// encoder::stream_lower_bound().
    ///
    /// @return The pointer to the popped symbol, it is the responsability
    /// of the caller to free or reuse the returned memory.
    auto pop_symbol() -> const uint8_t*;

    /// @param index The index of the symbol. Index must be between
    ///              encoder::stream_lower_bound() and
    ///              encoder::stream_upper_bound().
    ///
    /// @return The pointer of the symbol
    auto symbol_data(uint64_t index) const -> const uint8_t*;

    /// @param index The index of the symbol. Index must be between
    ///              encoder::stream_lower_bound() and
    ///              encoder::stream_upper_bound().
    ///
    /// @return The size of the symbol in bytes
    auto symbol_bytes(uint64_t index) const -> std::size_t;

    /// @return The maximum number of bytes per symbol. This is the maximum
    ///         number of bytes that can be pushed to the encoder.
    auto max_symbol_bytes() const -> std::size_t;

    /// Write an encoded symbol according to the given window and coding
    /// coefficients.
    /// @param symbol The buffer where the encoded symbol will be stored.
    ///        The encoder expects the symbol buffer to be
    ///        encoder::max_symbol_bytes() large.
    /// @param window The window of symbols to encode.
    /// @param coefficients The coding coefficients. A compatible format can
    ///        be created using the kodo::slide::generator::random_uniform
    ///        generator.
    /// @return The number of bytes written to the symbol buffer.
    std::size_t encode_symbol(uint8_t* symbol, range window,
                              const uint8_t* coefficients) const;

    /// Write a source symbol to the symbol buffer.
    /// @param symbol The buffer where the source symbol will be stored. The
    ///        symbol buffer must be encoder::symbol_bytes large.
    /// @param index The symbol index which should be written.
    /// @return The number of bytes written to the symbol buffer.
    std::size_t encode_systematic_symbol(uint8_t* symbol, uint64_t index) const;

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
