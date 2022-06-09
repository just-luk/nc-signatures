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
/// @brief Stack implementing a cauchy reed solomon generator
class rs_cauchy
{
public:
    /// Default constructor.
    rs_cauchy();

    /// @brief The generator constructor
    ///
    /// @param field the chosen finite field, only binary4 and binary8 is
    /// supported. In almost all cases binary8 is the field to choose.
    rs_cauchy(finite_field field);

    /// Copy constructor (disabled). This type is only movable.
    rs_cauchy(const rs_cauchy& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    rs_cauchy& operator=(const rs_cauchy& other) = delete;

    /// R-value copy constructor
    rs_cauchy(rs_cauchy&& other);

    /// R-value move assign operator
    rs_cauchy& operator=(rs_cauchy&& other);

    /// Destructor
    ~rs_cauchy();

    /// Configure the generator with the given parameters.
    /// This is useful for reusing an existing generator. Note that the
    /// reconfiguration always implies a reset, so the generator will be in a
    /// clean state after this operation
    ///
    /// @param symbols the number of symbols in a coding block
    void configure(std::size_t symbols);

    /// Resets the generator to the state when it was first configured.
    void reset();

    /// @return The configured finite field.
    auto field() const -> finite_field;

    /// @return The number of symbols supported by this generator.
    auto symbols() const -> uint64_t;

    /// @return The number of repair symbols supported by this generator.
    auto repair_symbols() const -> uint64_t;

    /// @return The number of remaining repair symbols supported by this
    ///         generator. This number decreases with each call to
    ///         rs_cauchy::generate().
    auto remaining_repair_symbols() const -> uint64_t;

    /// Generate the next set of coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @return The index of the generated coefficients. This can be used with
    ///         rs_cauchy::generate_specific() to recreate the coefficients at a
    ///         later time.
    auto generate(uint8_t* coefficients) -> std::size_t;

    /// Generate a specific set of coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param index The index of the coefficients to generate. The index must
    ///              be less than rs_cauchy::repair_symbols().
    void generate_specific(uint8_t* coefficients, uint64_t index) const;

    /// Return the maximum number of bytes to be generated when calling
    /// generate.
    auto max_coefficients_bytes() const -> std::size_t;

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
