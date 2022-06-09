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
#include <memory>

#include "../../finite_field.hpp"
#include "../../log_callback.hpp"
#include "../../version.hpp"
#include "../decoder.hpp"
namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace fulcrum
{
namespace generator
{
/// @brief Stack implementing an uniform random fulcrum block generator
class random_uniform
{
public:
    /// @brief The generator constructor
    random_uniform();

    /// Copy constructor (disabled). This type is only movable.
    random_uniform(const random_uniform& other) = delete;

    /// Copy assign operator (disabled). This type is only movable.
    random_uniform& operator=(const random_uniform& other) = delete;

    /// R-value copy constructor
    random_uniform(random_uniform&& other);

    /// R-value move assign operator
    random_uniform& operator=(random_uniform&& other);

    /// Destructor
    ~random_uniform();

    /// Configure the generator with the given parameters.
    /// This is useful for reusing an existing generator. Note that the
    /// reconfiguration always implies a reset, so the generator will be in a
    /// clean state after this operation
    ///
    /// @param symbols the number of symbols in a coding block
    /// @param expansion The number of expansion symbols to use
    void configure(uint64_t symbols, uint64_t expansion);

    /// Resets the generator to the state when it was first configured.
    void reset();

    /// @return The configured finite field.
    auto field() const -> finite_field;

    /// @return The number of symbols supported by this generator.
    auto symbols() const -> uint64_t;

    /// @return The expansion set for this generator
    auto expansion() const -> uint64_t;

    /// Generates the coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    void generate(uint8_t* coefficients);

    /// Partially generate the coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param symbols The number of symbols to generate coefficients for.
    ///                Must be less than or equal random_uniform::symbols().
    void generate_partial(uint8_t* coefficients, std::size_t symbols);

    /// Generate coefficients based on the decoder state.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param decoder The decoder to query for the current symbol state.
    void generate_recode(uint8_t* coefficients, const decoder& decoder);

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
