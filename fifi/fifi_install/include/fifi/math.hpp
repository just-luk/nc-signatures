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

#include <cstdint>
#include <memory>
#include <string>

#include "field_metric.hpp"
#include "finite_field.hpp"
#include "version.hpp"

#include "field/binary16_backend.hpp"
#include "field/binary4_backend.hpp"
#include "field/binary8_backend.hpp"
#include "field/prime2325_backend.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{

// Forward declaration
class impl;

/// This class provides a number of methods for performing operations in a
/// finite fields.
///
/// Mutiple different finite fields are available. See the fifi::finite_field
/// enum.
class math
{
public:
    /// Create an uninitialized math object
    math();

    /// Create new math object using the default field implementations
    /// @param the_field the field to use.
    math(finite_field the_field);

    /// Create new math object using the given backend
    /// @param backend the backend to use
    math(const field::binary4_backend* backend);

    /// Create new math object using the given backend
    /// @param backend the backend to use
    math(const field::binary8_backend* backend);

    /// Create new math object using the given backend
    /// @param backend the backend to use
    math(const field::binary16_backend* backend);

    /// Create new math object using the given backend
    /// @param backend the backend to use
    math(const field::prime2325_backend* backend);

    /// Copy constructor
    math(const math& other);

    /// Copy assign operator
    math& operator=(const math& other);

    /// R-value copy constructor
    math(math&& other);

    /// R-value move assign operator
    math& operator=(math&& other);

    /// Destructor
    ~math();

    /// Returns the number of field elements that can fit within a certain
    /// number of bytes
    /// @param size the number of bytes to store the field elements
    /// @return the number of field elements stored within the bytes
    std::size_t bytes_to_elements(std::size_t size) const;

    /// Returns the minimum number of bytes required to accommodate a certain
    /// number of field elements
    /// @param elements the number of field elements
    /// @return the size in bytes needed to store the field elements
    std::size_t elements_to_bytes(std::size_t elements) const;

    /// Function to access individual field elements in a buffer.
    /// This function assumes that values are packed.
    /// @param elements raw buffer that contains the field elements
    /// @param index index of element to access in the packed buffer
    /// @return the value of the element at specified index
    uint32_t get_value(const uint8_t* elements, std::size_t index) const;

    /// Function for assigning field elements a specific value in a buffer.
    /// This function assumes that values are packed.
    /// @param elements raw buffer that contains the elements to manipulate
    /// @param index index of element
    /// @param value The new value to assign to the element
    void set_value(uint8_t* elements, std::size_t index, uint32_t value) const;

    /// Returns the sum of two field elements.
    /// @param a The augend.
    /// @param b The addend.
    /// @return The sum of a and b.
    uint32_t add(uint32_t a, uint32_t b) const;

    /// Returns the difference of two field elements.
    /// @param a The minuend.
    /// @param b The subtrahend.
    /// @return The difference of a and b.
    uint32_t subtract(uint32_t a, uint32_t b) const;

    /// Returns the product of two field elements.
    /// @param a The multiplicand.
    /// @param b The multiplier.
    /// @return The product of a and b.
    uint32_t multiply(uint32_t a, uint32_t b) const;

    /// Returns the quotient of two field elements.
    /// @param a The numerator.
    /// @param b The denominator.
    /// @return The quotient of a and b.
    uint32_t divide(uint32_t a, uint32_t b) const;

    /// Returns inverse of a field element.
    /// @param a Element to invert.
    /// @return The inverse of the field element.
    uint32_t invert(uint32_t a) const;

    /// Adds two field element buffers.
    ///
    /// The operation is:
    ///
    ///     x[i] = x[i] + y[i]
    ///
    /// @param x Buffer storing the result and containing the first argument.
    /// @param y The buffer containing second argument.
    /// @param size The size of the provided buffers in bytes.
    void vector_add_into(uint8_t* x, const uint8_t* y, std::size_t size) const;

    /// Subtracts two field element buffers.
    ///
    /// The operation is:
    ///
    ///     x[i] = x[i] - y[i]
    ///
    /// @param x Buffer storing the result and containing the first argument.
    /// @param y The buffer containing second argument.
    /// @param size The size of the provided buffers in bytes.
    void vector_subtract_into(uint8_t* x, const uint8_t* y,
                              std::size_t size) const;

    /// Multiplies a field element buffer with a constant field element.
    ///
    /// The operation is:
    ///
    ///     x[i] = x[i] * constant
    ///
    /// @param x Buffer storing the result and containing the first argument.
    /// @param constant The constant to multiply with.
    /// @param size The size of the provided buffers in bytes.
    void vector_multiply_into(uint8_t* x, uint32_t constant,
                              std::size_t size) const;

    /// Adds a product of a field element buffer and a constant field element
    /// to an existing field element buffer.
    ///
    /// The operation is:
    ///
    ///     x[i] = x[i] + y[i] * constant
    ///
    /// @param x Buffer storing the result and containing the first argument.
    /// @param y The buffer containing second argument.
    /// @param constant The constant be multiplied with z.
    /// @param size The size of the provided buffers in bytes.
    void vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                  uint32_t constant, std::size_t size) const;

    /// Subtracts a product of a field element buffer and a constant field
    /// element from an existing field element buffer.
    ///
    /// The operation is:
    ///
    ///     x[i] = x[i] - y[i] * constant
    ///
    /// @param x Buffer storing the result and containing the first argument.
    /// @param y The buffer containing second argument.
    /// @param constant The constant be multiplied with z.
    /// @param size The size of the provided buffers in bytes.
    void vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                       uint32_t constant,
                                       std::size_t size) const;

    /// Calculates the dot product of the constants and the source
    /// vectors (y). The result is stored in the destination vectors (x).
    /// This variant should be optimal for any number of destination vectors.
    ///
    /// Consider the following setup:
    ///
    /// - 2 destination buffers (x)
    /// - 3 source buffers (y)
    /// - 2 constant buffers (c) each with 3 constants (one per source)
    ///
    /// The following calculations will be performed:
    ///
    ///     x[0] = c[0][0] * y[0] + c[0][1] * y[1] + c[0][2] * x[2]
    ///     x[1] = c[1][0] * y[0] + c[1][1] * y[1] + c[1][2] * x[2]
    ///
    /// @param x Buffers storing the results.
    /// @param y The buffers containing arguments.
    /// @param constants The buffer containing constants.
    /// @param size The size of the buffers in x and y.
    /// @param x_vectors The number of vectors in x and the number of constants
    ///        buffers.
    /// @param y_vectors The number of the number of vectors in y and constants
    ///        in the constants buffers.
    void vector_dot_product(uint8_t** x, const uint8_t** y,
                            const uint8_t** constants, std::size_t size,
                            std::size_t x_vectors, std::size_t y_vectors) const;

    /// @return The finite field used.
    finite_field field() const;

    /// @return The required granularity based on the finite field.
    uint32_t granularity() const;

    /// @return The highest value of the finite field.
    uint32_t max_value() const;

    /// @return The finite field's prime.
    uint32_t prime() const;

    /// Returns the total number of bytes processed by the this math object.
    /// @return The total number of bytes processed
    std::size_t total_processed() const;

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

    /// @return The number of metrics. This number must be equal to the number
    /// of members in the enum class
    auto metrics() const -> std::size_t;

    /// @return The metric id associated with the metric index
    auto metric_id(std::size_t index) const -> field_metric;

private:
    /// The finite field used
    finite_field m_field;

    /// The private implementation
    std::shared_ptr<impl> m_impl;
};

}
}
