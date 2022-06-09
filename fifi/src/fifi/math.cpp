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

#include "math.hpp"

#include "field/binary16_math.hpp"
#include "field/binary4_math.hpp"
#include "field/binary8_math.hpp"
#include "field/binary_math.hpp"
#include "field/prime2325_math.hpp"

#include "utils.hpp"

namespace fifi
{
inline namespace STEINWURF_FIFI_VERSION
{

class impl
{
public:
    virtual ~impl(){};

    virtual uint32_t add(uint32_t a, uint32_t b) const = 0;

    virtual uint32_t subtract(uint32_t a, uint32_t b) const = 0;

    virtual uint32_t multiply(uint32_t a, uint32_t b) const = 0;

    virtual uint32_t divide(uint32_t a, uint32_t b) const = 0;

    virtual uint32_t invert(uint32_t a) const = 0;

    virtual void vector_add_into(uint8_t* x, const uint8_t* y,
                                 std::size_t size) const = 0;

    virtual void vector_subtract_into(uint8_t* x, const uint8_t* y,
                                      std::size_t size) const = 0;

    virtual void vector_multiply_into(uint8_t* x, uint32_t constant,
                                      std::size_t size) const = 0;

    virtual void vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                          uint32_t constant,
                                          std::size_t size) const = 0;

    virtual void vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                               uint32_t constant,
                                               std::size_t size) const = 0;

    virtual void vector_dot_product(uint8_t** x, const uint8_t** y,
                                    const uint8_t** constants, std::size_t size,
                                    std::size_t x_vectors,
                                    std::size_t y_vectors) const = 0;

    virtual uint32_t granularity() const = 0;

    virtual uint32_t max_value() const = 0;

    virtual uint32_t prime() const = 0;

    virtual uint64_t total_processed() const = 0;

    virtual std::string metric_name(field_metric id) const = 0;

    virtual uint64_t metric_value(field_metric id) const = 0;

    virtual void copy_metrics_storage(uint8_t* data) const = 0;

    virtual std::size_t metrics_storage_bytes() const = 0;

    virtual std::string metrics_to_json() const = 0;

    virtual void reset_metrics() = 0;

    virtual void reset_metric(field_metric id) = 0;

    virtual std::size_t metrics() const = 0;

    virtual field_metric metric_id(std::size_t index) const = 0;
};

template <class T>
struct model : public impl
{

    model()
    {
    }

    template <class Backend>
    model(const Backend* backend) : m_math(backend)
    {
        assert(backend != nullptr);
    }

    uint32_t add(uint32_t a, uint32_t b) const override
    {
        return m_math.add(a, b);
    }

    uint32_t subtract(uint32_t a, uint32_t b) const override
    {
        return m_math.subtract(a, b);
    }

    uint32_t multiply(uint32_t a, uint32_t b) const override
    {
        return m_math.multiply(a, b);
    }

    uint32_t divide(uint32_t a, uint32_t b) const override
    {
        return m_math.divide(a, b);
    }

    uint32_t invert(uint32_t a) const override
    {
        return m_math.invert(a);
    }

    void vector_add_into(uint8_t* x, const uint8_t* y,
                         std::size_t size) const override
    {
        m_math.vector_add_into(x, y, size);
    }

    void vector_subtract_into(uint8_t* x, const uint8_t* y,
                              std::size_t size) const override
    {
        m_math.vector_subtract_into(x, y, size);
    }

    void vector_multiply_into(uint8_t* x, uint32_t constant,
                              std::size_t size) const override
    {
        m_math.vector_multiply_into(x, constant, size);
    }

    void vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                  uint32_t constant,
                                  std::size_t size) const override
    {
        m_math.vector_multiply_add_into(x, y, constant, size);
    }

    void vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                       uint32_t constant,
                                       std::size_t size) const override
    {
        m_math.vector_multiply_subtract_into(x, y, constant, size);
    }

    void vector_dot_product(uint8_t** x, const uint8_t** y,
                            const uint8_t** constants, std::size_t size,
                            std::size_t x_vectors,
                            std::size_t y_vectors) const override
    {
        m_math.vector_dot_product(x, y, constants, size, x_vectors, y_vectors);
    }

    uint32_t granularity() const override
    {
        return m_math.field_info().granularity;
    }

    uint32_t max_value() const override
    {
        return m_math.field_info().max_value;
    }

    uint32_t prime() const override
    {
        return m_math.field_info().prime();
    }

    uint64_t total_processed() const override
    {
        return m_math.total_processed();
    }

    std::string metric_name(field_metric id) const override
    {
        return m_math.metric_name(id);
    }

    uint64_t metric_value(field_metric id) const override
    {
        return m_math.metric_value(id);
    }

    void copy_metrics_storage(uint8_t* data) const override
    {
        m_math.copy_metrics_storage(data);
    }

    std::size_t metrics_storage_bytes() const override
    {
        return m_math.metrics_storage_bytes();
    }

    std::string metrics_to_json() const override
    {
        return m_math.metrics_to_json();
    }

    void reset_metrics() override
    {
        m_math.reset_metrics();
    }

    void reset_metric(field_metric id) override
    {
        m_math.reset_metric(id);
    }

    std::size_t metrics() const override
    {
        return m_math.metrics();
    }

    field_metric metric_id(std::size_t index) const override
    {
        return m_math.metric_id(index);
    }

    T m_math;
};

math::math() : m_field(finite_field::binary), m_impl(nullptr)
{
}

math::math(const math& other) : m_field(other.m_field), m_impl(other.m_impl)
{
}

math& math::operator=(const math& other)
{
    m_field = {other.m_field};
    m_impl = {other.m_impl};

    return *this;
}

math& math::operator=(math&& other)
{
    m_field = {other.m_field};
    m_impl = {std::move(other.m_impl)};

    return *this;
}

math::math(finite_field the_field) : m_field(the_field)
{
    switch (the_field)
    {
    case finite_field::binary:
        m_impl = std::make_shared<model<field::binary_math>>();
        break;
    case finite_field::binary4:
        m_impl = std::make_shared<model<field::binary4_math>>();
        break;
    case finite_field::binary8:
        m_impl = std::make_shared<model<field::binary8_math>>();
        break;
    case finite_field::binary16:
        m_impl = std::make_shared<model<field::binary16_math>>();
        break;
    case finite_field::prime2325:
        m_impl = std::make_shared<model<field::prime2325_math>>();
        break;
    default:
        assert(0 && "Not supported field");
    }
}

math::math(const field::binary4_backend* backend) :
    m_field(finite_field::binary4)
{
    m_impl = std::make_shared<model<field::binary4_math>>(backend);
}

math::math(const field::binary8_backend* backend) :
    m_field(finite_field::binary8)
{
    m_impl = std::make_shared<model<field::binary8_math>>(backend);
}

math::math(const field::binary16_backend* backend) :
    m_field(finite_field::binary16)
{
    m_impl = std::make_shared<model<field::binary16_math>>(backend);
}

math::math(const field::prime2325_backend* backend) :
    m_field(finite_field::prime2325)
{
    m_impl = std::make_shared<model<field::prime2325_math>>(backend);
}

math::math(math&& other) :
    m_field(other.m_field), m_impl(std::move(other.m_impl))
{
}

math::~math()
{
}

std::size_t math::bytes_to_elements(std::size_t size) const
{
    return fifi::bytes_to_elements(m_field, size);
}

std::size_t math::elements_to_bytes(std::size_t elements) const
{
    return fifi::elements_to_bytes(m_field, elements);
}

uint32_t math::get_value(const uint8_t* elements, std::size_t index) const
{
    return fifi::get_value(m_field, elements, index);
}

void math::set_value(uint8_t* elements, std::size_t index, uint32_t value) const
{
    return fifi::set_value(m_field, elements, index, value);
}

uint32_t math::add(uint32_t a, uint32_t b) const
{
    assert(m_impl);
    return m_impl->add(a, b);
}

uint32_t math::subtract(uint32_t a, uint32_t b) const
{
    assert(m_impl);
    return m_impl->subtract(a, b);
}

uint32_t math::multiply(uint32_t a, uint32_t b) const
{
    assert(m_impl);
    return m_impl->multiply(a, b);
}

uint32_t math::divide(uint32_t a, uint32_t b) const
{
    assert(m_impl);
    return m_impl->divide(a, b);
}

uint32_t math::invert(uint32_t a) const
{
    assert(m_impl);
    return m_impl->invert(a);
}

void math::vector_add_into(uint8_t* x, const uint8_t* y, std::size_t size) const
{
    assert(m_impl);
    m_impl->vector_add_into(x, y, size);
}

void math::vector_subtract_into(uint8_t* x, const uint8_t* y,
                                std::size_t size) const
{
    assert(m_impl);
    m_impl->vector_subtract_into(x, y, size);
}

void math::vector_multiply_into(uint8_t* x, uint32_t constant,
                                std::size_t size) const
{
    assert(m_impl);
    m_impl->vector_multiply_into(x, constant, size);
}

void math::vector_multiply_add_into(uint8_t* x, const uint8_t* y,
                                    uint32_t constant, std::size_t size) const
{
    assert(m_impl);
    m_impl->vector_multiply_add_into(x, y, constant, size);
}

void math::vector_multiply_subtract_into(uint8_t* x, const uint8_t* y,
                                         uint32_t constant,
                                         std::size_t size) const
{
    assert(m_impl);
    m_impl->vector_multiply_subtract_into(x, y, constant, size);
}

void math::vector_dot_product(uint8_t** x, const uint8_t** y,
                              const uint8_t** constants, std::size_t size,
                              std::size_t x_vectors,
                              std::size_t y_vectors) const
{
    assert(m_impl);
    m_impl->vector_dot_product(x, y, constants, size, x_vectors, y_vectors);
}

finite_field math::field() const
{
    assert(m_impl);
    return m_field;
}

uint32_t math::granularity() const
{
    assert(m_impl);
    return m_impl->granularity();
}

uint32_t math::max_value() const
{
    assert(m_impl);
    return m_impl->max_value();
}

uint32_t math::prime() const
{
    assert(m_impl);
    return m_impl->prime();
}

std::size_t math::total_processed() const
{
    assert(m_impl);
    return m_impl->total_processed();
}

std::string math::metric_name(field_metric id) const
{
    assert(m_impl);
    return m_impl->metric_name(id);
}

uint64_t math::metric_value(field_metric id) const
{
    assert(m_impl);
    return m_impl->metric_value(id);
}

void math::copy_metrics_storage(uint8_t* data) const
{
    assert(m_impl);
    m_impl->copy_metrics_storage(data);
}

std::size_t math::metrics_storage_bytes() const
{
    assert(m_impl);
    return m_impl->metrics_storage_bytes();
}

std::string math::metrics_to_json() const
{
    assert(m_impl);
    return m_impl->metrics_to_json();
}

void math::reset_metrics()
{
    assert(m_impl);
    m_impl->reset_metrics();
}

void math::reset_metric(field_metric id)
{
    assert(m_impl);
    m_impl->reset_metric(id);
}

std::size_t math::metrics() const
{
    assert(m_impl);
    return m_impl->metrics();
}

field_metric math::metric_id(std::size_t index) const
{
    assert(m_impl);
    return m_impl->metric_id(index);
}

}
}
