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

#include <memory>

#include "../../finite_field.hpp"
#include "../../log_callback.hpp"
#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace block
{
namespace generator
{
/// @brief Stack implementing a 2D parity FEC generator.
///
/// This generator produces the coefficients used for generating the FEC
/// in many different FEC schemes, including:
///
/// * Pro-MPEG FEC,
/// * SMPTE-2022-1 FEC https://ieeexplore.ieee.org/document/7291470, and
/// * Flexible FEC (or "FLEX FEC") https://tools.ietf.org/html/rfc8627.
///
/// .. warning::
///     When using this generator with a kodo::block::encoder or
///     kodo::block::decoder, use the `binary` kodo::finite_field.
///
class parity_2d
{
public:
    /// Default constructor
    parity_2d();

    /// Copy constructor (disabled). This type is only movable.
    parity_2d(const parity_2d& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    parity_2d& operator=(const parity_2d& other) = delete;

    /// R-value copy constructor
    parity_2d(parity_2d&& other);

    /// R-value move assign operator
    parity_2d& operator=(parity_2d&& other);

    /// Destructor
    ~parity_2d();

    /// Configure the generator with the given parameters.
    /// This is useful for reusing an existing generator.
    ///
    /// @param rows the number of rows, must be larger than 0.
    /// @param columns the number of columns, must be larger than 0.
    void configure(uint64_t rows, uint64_t columns);

    /// Resets the generator to the state when it was first configured.
    void reset();

    /// @return The number of symbols, this is equal to parity_2d::rows() *
    /// parity_2d::columns().
    auto symbols() const -> uint64_t;

    /// @return The number of rows.
    auto rows() const -> uint64_t;

    /// @return The number of columns.
    auto columns() const -> uint64_t;

    /// @return false if the generator reaches beyond the chosen amount of
    /// source and repair packets, otherwise true.
    auto can_advance() const -> bool;

    /// Advances the generator to the next step of the repair process.
    void advance();

    /// Returns true if the generator can generate the coefficients at this
    /// point.
    auto can_generate() const -> bool;

    /// Generate the coefficients.
    /// Use parity_2d::can_generate() to check if calling this function is
    /// allowed for this position.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @return The position for which the coefficients were generated. This can
    ///         be used for generating the same coefficients with
    ///         parity_2d::generate_specific(). The maximum for this value can
    ///         be calculated like so: `rows * columns + (rows * columns)`.
    auto generate(uint8_t* coefficients) const -> std::size_t;

    /// Generate coefficients for a specific position. This position is
    /// returned by parity_2d::generate().
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param position The specific position for which the coefficients
    ///                 should be generated.
    void generate_specific(uint8_t* coefficients, std::size_t position) const;

    /// Return the maximum number of bytes to be generated when calling
    /// generate.
    auto max_coefficients_bytes() const -> std::size_t;

    /// Specifies if the generator should generate column redundancy or not.
    /// @param enabled A bool. true is enabled and false is disabled.
    void set_column_redundancy_enabled(bool enabled);

    /// @return The argument called by
    /// parity_2d::set_column_redundancy_enabled().
    ///         true is default.
    auto column_redundancy_enabled() const -> bool;

    /// Specifies if the generator should generate row redundancy or not.
    /// @param enabled A bool. true is enabled and false is disabled.
    void set_row_redundancy_enabled(bool enabled);

    /// @return The argument called by parity_2d::set_row_redundancy_enabled().
    ///         true is default.
    auto row_redundancy_enabled() const -> bool;

    /// Enables the log.
    /// @param callback The callback which handles the log message.
    /// @param user_data A pointer to be passed to the callback when triggered.
    ///                  nullptr can be used if left unused.
    void enable_log(const log_callback& callback, void* user_data);

    /// Disables the log.
    void disable_log();

    /// @return true if log is enabled, otherwise False.
    auto is_log_enabled() const -> bool;

    /// Set a log name which will be included with log messages produced by
    /// this object.
    /// @param name The log name to set.
    void set_log_name(const std::string& name);

    /// @return The log name assigned to this object.
    auto log_name() const -> std::string;

public:
    // Implementation detail
    struct interface;

private:
    /// Pointer to the implementation
    std::unique_ptr<interface> m_impl;
};
}
}
}
}
