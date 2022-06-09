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
/// @brief Stack implementing an uniform random block generator
///
/// This stack does not provide a payload API, the user can directly access
/// the encoded symbol and the corresponding coding vector.
class tunable
{
public:
    /// Default constructor
    tunable();

    /// @brief The generator constructor
    ///
    /// @param field the chosen finite field
    tunable(finite_field field);

    /// Copy constructor (disabled). This type is only movable.
    tunable(const tunable& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    tunable& operator=(const tunable& other) = delete;

    /// R-value copy constructor
    tunable(tunable&& other);

    /// R-value move assign operator
    tunable& operator=(tunable&& other);

    /// Destructor
    ~tunable();

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

    /// Generates the coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param density a value between 1.0 and 0.0 which determines the density
    ///                of the generated coefficients.
    ///                The number of coefficients generated is calculated like
    ///                so max(1, symbols * density).
    void generate(uint8_t* coefficients, float density);

    /// Partially generate the coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param symbols The number of symbols to generate coefficients for.
    ///                Must be less than or equal tunable::symbols().
    /// @param density a value between 1.0 and 0.0 which determines the density
    ///                of the generated coefficients.
    ///                The number of coefficients generated is calculated like
    ///                so max(1, symbols * density).
    void generate_partial(uint8_t* coefficients, uint64_t symbols,
                          float density);

    /// Sets the state of the coefficient generator. The coefficient generator
    /// will always produce the same set of coefficients for a given seed
    /// @param seed The seed.
    void set_seed(uint64_t seed);

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
