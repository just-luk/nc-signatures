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

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

#include <kodo/block/decoder.hpp>
#include <kodo/block/encoder.hpp>
#include <kodo/block/generator/random_uniform.hpp>
#include <kodo/finite_field.hpp>
#include <kodo/fulcrum/decoder.hpp>
#include <kodo/fulcrum/encoder.hpp>
#include <kodo/fulcrum/generator/random_uniform.hpp>
#include <kodo/to_string.hpp>
#include <kodo/version.hpp>

struct stats
{
    stats(const std::vector<std::size_t>& data) :
        count(data.size()), min(*std::min_element(data.begin(), data.end())),
        max(*std::max_element(data.begin(), data.end())),
        nonzero(std::count_if(data.begin(), data.end(),
                              [](auto v) { return v > 0; })),
        average(1.0 * std::accumulate(data.begin(), data.end(), 0) /
                data.size())
    {
    }

    std::size_t count;
    std::size_t min;
    std::size_t max;
    std::size_t nonzero;
    double average;
};

std::ostream& operator<<(std::ostream& os, const stats& obj)
{
    os << "min:     " << obj.min << "\n";
    os << "max:     " << obj.max << "\n";
    os << "nonzero: " << obj.nonzero << "/" << obj.count << "\n";
    os << "average: " << obj.average << "\n";
    return os;
}

template <class Encoder, class Decoder, class Generator>
stats calculate_linear_dependency(Encoder& encoder, Decoder& decoder,
                                  Generator& generator, std::size_t runs)
{
    std::vector<uint8_t> data_in(encoder.block_bytes());
    std::generate(data_in.begin(), data_in.end(), rand);

    std::vector<uint8_t> symbol(encoder.symbol_bytes());
    std::vector<uint8_t> coefficients(generator.max_coefficients_bytes());

    std::vector<std::size_t> data_points;
    for (std::size_t i = 0; i < runs; i++)
    {
        encoder.reset();
        decoder.reset();
        generator.reset();

        std::vector<uint8_t> data_out(decoder.block_bytes());

        encoder.set_symbols_storage(data_in.data());
        decoder.set_symbols_storage(data_out.data());

        generator.set_seed(rand());

        std::size_t generated_symbols = 0;
        while (!decoder.is_complete())
        {
            generator.generate(coefficients.data());
            encoder.encode_symbol(symbol.data(), coefficients.data());
            decoder.decode_symbol(symbol.data(), coefficients.data());
            generated_symbols += 1;
        }

        data_points.push_back(generated_symbols - encoder.symbols());
    }
    return stats{data_points};
}

/// @example linear_dependency.cpp
///
/// Example which shows how different codecs and configurations affect the
/// linear dependency.
int main()
{
    srand(static_cast<uint32_t>(time(0)));

    auto runs = 10000;
    auto symbols = 150;
    auto symbol_bytes = 2;

    for (auto field :
         {kodo::finite_field::binary, kodo::finite_field::binary4,
          kodo::finite_field::binary8, kodo::finite_field::binary16})
    {
        std::cout << "block (" << kodo::to_string(field) << ")" << std::endl;

        kodo::block::encoder encoder(field);
        kodo::block::decoder decoder(field);
        encoder.configure(symbols, symbol_bytes);
        decoder.configure(symbols, symbol_bytes);

        kodo::block::generator::random_uniform generator(field);
        generator.configure(encoder.symbols());

        std::cout << calculate_linear_dependency(encoder, decoder, generator,
                                                 runs)
                  << std::endl;
    }

    for (auto field : {kodo::finite_field::binary4, kodo::finite_field::binary8,
                       kodo::finite_field::binary16})
    {
        std::cout << "fulcrum (" << kodo::to_string(field) << ")" << std::endl;

        auto expansion = 10;
        kodo::fulcrum::encoder encoder(field);
        kodo::fulcrum::decoder decoder(field);
        encoder.configure(symbols, symbol_bytes, expansion);
        decoder.configure(symbols, symbol_bytes, expansion);

        kodo::fulcrum::generator::random_uniform generator;
        generator.configure(encoder.symbols(), expansion);

        std::cout << calculate_linear_dependency(encoder, decoder, generator,
                                                 runs)
                  << std::endl;
    }
}
