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

#include "../log_callback.hpp"
#include "../version.hpp"
#include "width.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{
/// A perpetual encoder
///
/// The goal of this implementation is to support millions of symbols in a
/// single block. To implement this efficiently we need to "limit" the size
/// of the equation system we need to solve.
///
/// Let's consider the following example:
///
/// Symbols: 100000
///
/// Field: 2^8
///
/// In this case we have an encoding vector of 100000 bytes. However,
/// to limit the size of the problem we need to specify a maximum "width" of
/// the encoding vector, in this example we use a width of 4 symbols.
///
/// Encoding works by picking a random offset e.g. i and only including
/// the next i+3 symbols. E.g. say we pick 1272. Then the encoding
/// vector will include 1272, 1273, 1274, 1275. All other coefficients
/// will be zero.
///
/// The advantage of this type of encoding is that we can represent the
/// decoding matrix in a very space efficient manner. Since for every
/// pivot symbol we only need to store 4 bytes.
///
/// It would look something like this:
///
///              bytes     0    1    2    3    4    5    6    7   . . .
///                     +----+----+----+----+
///         symbol   0  |    |    |    |    |
///                     +-------------------+
///                          +-------------------+
///                  1       |    |    |    |    |
///                          +-------------------+
///                               +-------------------+
///                  2            |    |    |    |    |
///                               +-------------------+
///                                    +-------------------+
///                  3                 |    |    |    |    |
///                                    +-------------------+
///                                         +-------------------+
///                  4                      |    |    |    |    |  . .
///                                         +----+----+----+----+
///                  .
///                  .                                .
///                  .                                .
///
///
/// Lets see how decoding works. Assume we already have a symbol
/// stored at position 3:
///
///                       +----+----+----+----+
///     symbol 3          |    |    |    |    |
///                       +-------------------+
///
/// Now another symbol with pivot at position 3 arrives:
///
///                       +----+----+----+----+
///     symbol 3          |    |    |    |    |
///                       +-------------------+
///       subtract from     |
///                         v
///                       +----+----+----+----+
///     new symbol at 3   |    |    |    |    |
///                       +-------------------+
///                              =
///     result with position 3 +----+----+----+
///     eliminated             |    |    |    |
///                            +--------------+
///
/// We can now continue elimination as if the new symbol has a pivot at
/// position 4.
///
/// By shifting it down one byte and zeroing the top byte:
///
///                       +----+----+----+----+
///     new symbol at 4   |    |    |    |  0 |
///                       +-------------------+
///
class encoder
{
public:
    /// Default constructor
    encoder();

    /// @brief The encoder constructor
    ///
    /// @param width the chosen finite field
    encoder(kodo::perpetual::width width);

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
    /// @param block_bytes The size of the block in bytes
    /// @param symbol_bytes The size of a symbol in bytes
    /// @param outer_interval The interval between outer code symbols. As an
    /// example an interval of 8 means that there are 8 data symbols between
    /// each outer code symbol
    /// @param outer_segments The number of outer code segments - the outer code
    /// coding vector is composed of N number of *width* bit segments. This
    /// also means that the number of mixed symbols in an outer code symbol is
    /// outer_segments * width. The default width for this perpetual code is 32.
    void configure(std::size_t block_bytes, std::size_t symbol_bytes,
                   uint64_t outer_interval = 8, std::size_t outer_segments = 8);

    /// Reset the state of the encoder.
    void reset();

    /// @return The total number of symbols, including zero symbols, data
    /// symbols and outer symbols.
    auto symbols() const -> uint64_t;

    /// @return The number of data symbols
    auto data_symbols() const -> uint64_t;

    /// @return The number of outer symbols
    auto outer_symbols() const -> uint64_t;

    /// @return The size in bytes per symbol supported by this encoder
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

    /// @return The number of bytes that can be encoded with this encoder.
    auto block_bytes() const -> std::size_t;

    /// Initialize all the symbols in the block.
    ///
    /// @param symbol_storage The buffer containing all the data for the the
    ///        block
    void set_symbols_storage(const uint8_t* symbol_storage);

    /// @return The memory for the block.
    auto symbols_storage() const -> const uint8_t*;

    /// Creates a new encoded symbol given the passed encoding coeffcients.
    ///
    /// @param symbol_storage This is the memory where the encoded symbol
    ///        will be produced. This buffer must be large enough to store
    ///        symbol_bytes() bytes.
    /// @param coefficients These are the coding coefficients
    /// @param offset The offset of the coding coefficients
    void encode_symbol(uint8_t* symbol_storage, uint64_t coefficients,
                       uint64_t offset) const;

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
