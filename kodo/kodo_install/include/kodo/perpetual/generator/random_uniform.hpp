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

#include "../../log_callback.hpp"
#include "../../version.hpp"
#include "../width.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace perpetual
{
namespace generator
{
/// @brief Stack implementing an uniform random perpetual generator
class random_uniform
{
public:
    /// Default constructor
    random_uniform();

    /// @brief The decoder constructor
    ///
    /// @param width the chosen finite field
    random_uniform(kodo::perpetual::width width);

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

    /// @return the width.
    auto width() const -> uint64_t;

    /// Generates the coefficients.
    /// @return the generated coefficients as an integer where each bit
    /// represents a coefficient.
    auto generate(uint64_t seed) -> uint64_t;

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
