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

#include "width.hpp"

#include "../log_callback.hpp"
#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{
/// @brief Stack implementing a plain RLNC symbol decoder.
///
/// This stack does not provide a payload API, the user can directly access
/// the encoded symbol and the corresponding coding vector.
class decoder
{
public:
    /// Default constructor
    decoder();

    /// @brief The decoder constructor
    ///
    /// @param width the chosen finite field
    decoder(kodo::perpetual::width width);

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
    /// @param block_bytes The size of the block in bytes
    /// @param symbol_bytes The size of a symbol in bytes
    /// @param outer_interval The interval between outer code symbols. As an
    /// example an interval of 8 means that there are 8 data symbols between
    /// each outer code symbol
    /// @param outer_segments The number of outer code segments - the outer code
    /// coding vector is composed of N number of *width* bit segments. This
    /// also means that the number of mixed symbols in an outer code symbol is
    /// outer_segments * width.
    /// @param mapping_threshold How much of the inner code needs to be fill
    /// before the process of mapping to the outer code is started. The value is
    /// in the interval ]0, 1].
    void configure(std::size_t block_bytes, std::size_t symbol_bytes,
                   uint64_t outer_interval = 8, std::size_t outer_segments = 8,
                   double mapping_threshold = 0.98);

    /// Reset the state of the decoder.
    void reset();

    /// @return The total number of symbols, including zero symbols, data
    /// symbols and outer symbols.
    auto symbols() const -> uint64_t;

    /// @return The number of data symbols
    auto data_symbols() const -> uint64_t;

    /// @return The number of outer symbols
    auto outer_symbols() const -> uint64_t;

    /// @return The size in bytes per symbol supported by this decoder
    auto symbol_bytes() const -> std::size_t;

    /// @return the width of the encoding, i.e., now many symbols are combined
    /// for each encoded inner symbol.
    auto width() const -> uint64_t;

    /// @return The interval between outer code symbols. As an example an
    /// interval of 8 means that there are 8 data symbols between each outer
    /// code symbol, if the outer interval is 0 no outer symbols exists.
    auto outer_interval() const -> uint64_t;

    /// @return The number of outer code segments - the outer code coding vector
    /// is composed of N number of decoder::width() bit segments. This also
    /// means that the number of mixed symbols in an outer code symbol is
    /// decoder::outer_segments() * decoder::width().
    auto outer_segments() const -> std::size_t;

    /// @return How much of the inner code needs to be fill before the process
    /// of mapping to the outer code is started. The value is in the interval
    /// ]0, 1].
    auto mapping_threshold() const -> double;

    /// @return the total number of bytes that can be decoded with this decoder.
    auto block_bytes() const -> std::size_t;

    /// Initialize all the symbols in the block.
    ///
    /// @param symbol_storage The buffer containing all the data for the the
    ///        block.
    void set_symbols_storage(uint8_t* symbol_storage);

    /// @return The memory for the block.
    auto symbols_storage() const -> uint8_t*;

    /// Feed a coded symbol to the decoder.
    ///
    /// @param symbol The data of the symbol assumed to be
    ///               decoder::symbol_bytes() bytes in size.
    /// @param coefficients The coding coefficients that describe the encoding
    ///                     performed on the symbol.
    /// @param offset The offset of the coding coefficients
    void decode_symbol(uint8_t* symbol, uint64_t coefficients, uint64_t offset);

    /// @return True if a new pivot was found during decoder::decode_symbol()
    ///         otherwise return false.
    auto pivot_found() const -> bool;

    /// @return Index of the newest pivot found.
    auto pivot() const -> uint64_t;

    /// @return True if the decoder contains a pivot at the specific index.
    auto is_pivot(uint64_t index) const -> bool;

    /// @return The rank of the decoder
    auto rank() const -> uint64_t;

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
