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

#include <cassert>
#include <cstdint>
#include <cstring>

#include <ranbo/xoshiro256ss.h>

#include "../version.hpp"

#include "byte_count.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

/// This layer generates the coding coefficients used for the encoding /
/// decoding.
template <class Super>
struct layer_random_uniform : public Super
{
    template <class Config>
    void configure(const Config& config)
    {
        Super::configure(config);
        set_seed(0);
    }

    /// The data type used to store random values
    using result_type = uint64_t;

    /// @param seed Seed the generator. Using the same seed on the encoder
    ///        and decoder side will ensure that the same id is generated.
    void set_seed(uint64_t seed)
    {
        ranbo_xoshiro256ss_set_seed(&m_generator, seed);
    }

    /// Generates the random coefficients.
    /// @param data Pointer to memory where the random coefficients should be
    ///        generated.
    /// @param bytes The number of bytes to generate.
    void generate_data(uint8_t* data, byte_count bytes)
    {
        assert(data != nullptr);
        assert(bytes > byte_count{0U});

        while (bytes >= byte_count{sizeof(result_type)})
        {
            result_type value = ranbo_xoshiro256ss_generate(&m_generator);

            // Compilers should be able to elide the call to
            // memcpy here.
            std::memcpy(data, &value, sizeof(result_type));

            data += sizeof(result_type);
            bytes -= byte_count{sizeof(result_type)};
        }

        assert(bytes < byte_count{sizeof(result_type)});

        if (bytes > byte_count{0})
        {
            result_type value = ranbo_xoshiro256ss_generate(&m_generator);
            // Compilers should be able to elide the call to
            // memcpy here.
            std::memcpy(data, &value, bytes.value);
        }
    }

private:
    /// The random generator used.
    ranbo_xoshiro256ss m_generator;
};
}
}
}
