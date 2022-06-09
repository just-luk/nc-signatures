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

#include "../decoder.hpp"
#include "../encoder.hpp"
#include "../range.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{
namespace generator
{

/// @brief The coefficient generator for the RLNC sliding window codec.
class random_uniform
{
public:
    /// Default constructor
    random_uniform();

    /// @brief The encoder constructor
    ///
    /// @param field the chosen finite field
    random_uniform(finite_field field);

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

    /// @return The configured finite field.
    auto field() const -> finite_field;

    /// Generates the coefficients.
    /// @param coefficients The data pointer where the coefficients will be
    ///                     written.
    /// @param window The coding window.
    void generate(uint8_t* coefficients, range window);

    /// Sets the state of the coefficient generator. The coefficient generator
    /// will always produce the same set of coefficients for a given seed.
    /// @param seed The seed.
    void set_seed(uint64_t seed);

    /// Return the number of bytes to be generated based on the given window
    /// @param window The coding window.
    auto coefficients_bytes(range window) const -> std::size_t;

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
