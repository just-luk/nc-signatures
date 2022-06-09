// License for Commercial Usage
// Distributed under the "FIFI EVALUATION LICENSE 1.3"
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
// Distributed under the "FIFI RESEARCH LICENSE 1.2"
//
// Licensees holding a valid research license may use this project
// in accordance with the license agreement terms provided with the
// Software
//
// See accompanying file LICENSE.rst or https://www.steinwurf.com/license

#pragma once

#include "../detail/cpu_support.hpp"
#include "../detail/enabled_ptr.hpp"
#include "../detail/field_metrics.hpp"
#include "../finite_field.hpp"
#include "../version.hpp"

#include "binary16_backend.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{
namespace field
{

/// Finite field operations in the binary extension field 2^16.
class binary16_math
{
public:
    /// The backend used
    using backend_type = binary16_backend;

    /// The finite field
    using field_type = binary16;

public:
    /// Constructor
    binary16_math();

    /// Constructor
    binary16_math(const binary16_backend* backend);

    /// Constructor
    binary16_math(const binary16_backend* backend,
                  const detail::cpu_support& cpu);

    /// Change the backend used
    void set_backend(const binary16_backend* backend);
    void set_backend(const binary16_backend* backend,
                     const detail::cpu_support& cpu);

    /// @copydoc acceleration::field()
    finite_field field() const;

    /// @copydoc acceleration::field_info()
    binary16 field_info() const;

    /// @copydoc math::add()
    uint32_t add(uint32_t a, uint32_t b) const;

    /// @copydoc math::subtract()
    uint32_t subtract(uint32_t a, uint32_t b) const;

    /// @copydoc math::multiply()
    uint32_t multiply(uint32_t a, uint32_t b) const;

    /// @copydoc math::divide()
    uint32_t divide(uint32_t a, uint32_t b) const;

    /// @copydoc math::invert()
    uint32_t invert(uint32_t a) const;

    /// @copydoc math::vector_add_into()
    void vector_add_into(uint8_t* x, const uint8_t* y, std::size_t size) const;

    /// @copydoc math::vector_subtract_into()
    void vector_subtract_into(uint8_t* x, const uint8_t* y,
                              std::size_t size) const;

    /// @copydoc math::vector_multiply_into()
    void vector_multiply_into(uint8_t* x, uint32_t constant,
                              std::size_t size) const;

    /// @copydoc math::vector_multiply_add_into()
    void vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                  uint32_t constant, std::size_t size) const;

    /// @copydoc math::vector_multiply_subtract_into()
    void vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                       uint32_t constant,
                                       std::size_t size) const;

    /// @copydoc math::vector_dot_product()
    void vector_dot_product(uint8_t** x, const uint8_t** y,
                            const uint8_t** constants, std::size_t size,
                            std::size_t x_vectors, std::size_t y_vectors) const;

    /// @copydoc math::total_processed()
    uint64_t total_processed() const;

    /// @return The name of a metric as a string
    auto metric_name(field_metric id) const -> std::string;

    /// @return A specific metric value
    auto metric_value(field_metric id) const -> uint64_t;

    /// @return a memory copy of the metrics storage.
    void copy_metrics_storage(uint8_t* data) const;

    /// @return the size of the memory allocated for the metrics in bytes
    auto metrics_storage_bytes() const -> std::size_t;

    /// @return All metrics in json format.
    auto metrics_to_json() const -> std::string;

    /// Reset all the metrics
    void reset_metrics();

    /// Reset specific metric
    void reset_metric(field_metric id);

    /// @return The number of metrics in the metrics object
    auto metrics() const -> std::size_t;

    /// @return The metric id associated with the metric index
    auto metric_id(std::size_t index) const -> field_metric;

private:
    /// The acceleration backend
    const binary16_backend* m_backend;

    /// Binary AVX2 acceleration
    detail::enabled_ptr<binary_avx2> m_binary_avx2;

    /// Binary SSE2 acceleration
    detail::enabled_ptr<binary_sse2> m_binary_sse2;

    /// Binary basic acceleration
    detail::enabled_ptr<binary_basic> m_binary_basic;

    /// Binary16 basic acceleration
    detail::enabled_ptr<binary16_basic> m_binary16_basic;

    /// Object containing metrics for the accelerations
    abacus::metrics m_metrics;

    /// metrics to manipulate the metrics object
    mutable detail::field_metrics m_field_metrics;
};

}
}
}
