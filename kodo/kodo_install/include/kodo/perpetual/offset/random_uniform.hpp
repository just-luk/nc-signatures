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
namespace perpetual
{
namespace offset
{
/// Generates a random uniform offset for the perpetual code
class random_uniform
{
public:
    /// Default constructor
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

    /// Configure the offset generator with the given parameters.
    /// This is useful for reusing an existing coder. Note that the
    /// reconfiguration always implies a reset, so the coder will be in a
    /// clean state after this operation
    ///
    /// @param symbols the number of symbols in a coding block
    void configure(uint64_t symbols);

    /// @return The number of symbols supported by this generator.
    auto symbols() const -> uint64_t;

    /// @return The next offset
    uint64_t offset();

    /// Set the seed for the offset generator.
    /// @param seed The seed.
    void set_seed(uint64_t seed);

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
