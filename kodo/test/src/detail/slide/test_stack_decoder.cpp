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

#include <kodo/detail/slide/stack_decoder.hpp>

#include <ctime>

#include <fifi/field/binary8_math.hpp>
#include <fifi/utils.hpp>
#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
using binary8_stack = slide::stack_decoder<fifi::field::binary8_math>;

/// Very simple struct which represents the coding values of decoder
struct matrix
{
    /// @param lower_bound This is the stream lower bound
    /// @param rows These are the coefficient values rows of the matrix
    matrix(std::size_t lower_bound,
           std::vector<std::vector<std::size_t>> rows) :
        m_lower_bound(lower_bound),
        m_rows(rows)
    {
    }

    std::size_t m_lower_bound;
    std::vector<std::vector<std::size_t>> m_rows;
};

/// Output operator for printing out the matrix
std::ostream& operator<<(std::ostream& out, const matrix& m)
{

    for (std::size_t i = 0; i < m.m_rows.size(); ++i)
    {
        auto& row = m.m_rows[i];

        if (i == 0)
        {
            out << "\n      ";
            for (std::size_t j = 0; j < row.size(); ++j)
            {
                out << std::setw(3) << j + m.m_lower_bound << " ";
            }
            out << "\n";
        }

        out << std::setw(2) << i + m.m_lower_bound << ": [ ";

        for (std::size_t v : row)
        {
            out << std::setw(3) << v << " ";
        }
        out << "]\n";
    }

    return out;
}

/// Compares to matrices
bool operator==(const matrix& m1, const matrix& m2)
{
    return (m1.m_rows == m2.m_rows) && (m1.m_lower_bound == m2.m_lower_bound);
}

/// Construct a matrix from a decoder
template <class DecoderStack>
matrix to_matrix(DecoderStack& decoder)
{
    auto range = decoder.stream_range();

    std::vector<std::vector<std::size_t>> values;

    for (symbol_index i : range)
    {
        std::vector<std::size_t> row;

        if (decoder.has_vector(i))
        {
            auto view = decoder.coefficients_data(i);
            auto frame = decoder.coefficients_view_to_symbol_frame(view);

            for (symbol_index j : range)
            {
                if (in_frame(frame, j))
                {
                    row.push_back(decoder.get_value(view, j));
                }
                else
                {
                    row.push_back(0U);
                }
            }
        }
        else
        {
            for (symbol_index j : range)
            {
                (void)j;
                row.push_back(0U);
            }
        }

        values.push_back(row);
    }

    return matrix(decoder.stream_range().lower_bound().value, values);
}
}

TEST(detail_slide_test_stack_decoder, decoding_algorithm_binary8)
{
    byte_count max_symbol_bytes{4U};
    symbol_count symbols{6U};

    // Create the config object
    binary8_stack::config config;
    config.max_symbol_bytes = max_symbol_bytes;
    // Create the decoder
    binary8_stack decoder;

    decoder.configure(config);

    // We need to provide the symbol storage for the decoder
    std::vector<std::vector<uint8_t>> decoder_storage(
        symbols.value, std::vector<uint8_t>(max_symbol_bytes.value));

    // Provide the decoder with the storage
    for (std::size_t i = 0; i < symbols.value; ++i)
    {
        decoder.push_symbol();
    }

    EXPECT_EQ(symbol_count{6U}, decoder.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, decoder.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, decoder.symbols_decoded());

    std::vector<uint8_t> coefficients;

    // Lets start by adding a zero symbol
    symbol_range window{symbol_index{2U}, symbol_index{4U}};
    coefficients.resize(2);

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(), {0, 0});

    uint8_t* claimed =
        decoder.decode_symbol(decoder_storage[0].data(), max_symbol_bytes,
                              window, coefficients.data());

    // Symbol is linear dept. so we get it back
    EXPECT_EQ(claimed, decoder_storage[0].data());

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    window = symbol_range{symbol_index{0U}, symbol_index{2U}};
    coefficients.resize(2);

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(), {1, 0});

    claimed = decoder.decode_symbol(decoder_storage[0].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(4);
    window = symbol_range{symbol_index{0U}, symbol_index{4U}};

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 0, 1, 1});

    claimed = decoder.decode_symbol(decoder_storage[1].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(4);
    window = symbol_range{symbol_index{0U}, symbol_index{4U}};

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 1, 0, 1});

    claimed = decoder.decode_symbol(decoder_storage[2].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 1, 0, 0}, // decoder_storage[2]
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(5);
    window = symbol_range{symbol_index{1U}, symbol_index{5U}};

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 1, 0, 1});

    claimed = decoder.decode_symbol(decoder_storage[3].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 1, 0, 0}, // decoder_storage[2]
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Move the stream
    claimed = decoder.pop_symbol();

    // We get back the initial symbol
    EXPECT_EQ(claimed, decoder_storage[0].data());

    decoder.push_symbol();

    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));

    {
        std::size_t lower_bound = 1U;
        matrix m1(lower_bound,
                  //  index:  1  2  3  4  5  6
                  {{1, 0, 1, 0, 0, 0}, // decoder_storage[2]
                   {0, 1, 1, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 1, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(4);
    window = symbol_range{symbol_index{1U}, symbol_index{5U}};

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 1, 0, 1});

    claimed = decoder.decode_symbol(decoder_storage[0].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // Symbol is linear dept. so we get it back
    EXPECT_EQ(claimed, decoder_storage[0].data());

    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));

    {
        std::size_t lower_bound = 1U;
        matrix m1(lower_bound,
                  //  index:  1  2  3  4  5  6
                  {{1, 0, 1, 0, 0, 0}, // decoder_storage[2]
                   {0, 1, 1, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 1, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(2);
    window = symbol_range{symbol_index{3U}, symbol_index{5U}};

    fifi::set_values(fifi::finite_field::binary8, coefficients.data(), {1, 1});

    claimed = decoder.decode_symbol(decoder_storage[0].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));

    {
        std::size_t lower_bound = 1U;
        matrix m1(lower_bound,
                  //  index:  1  2  3  4  5  6
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[2]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 1, 0, 0, 0}, // decoder_storage[0]
                   {0, 0, 0, 1, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Move the stream
    claimed = decoder.pop_symbol();
    EXPECT_EQ(claimed, decoder_storage[2].data());

    decoder.push_symbol();

    claimed = decoder.pop_symbol();
    EXPECT_EQ(claimed, decoder_storage[1].data());

    decoder.push_symbol();

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{7}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{8}));

    {
        std::size_t lower_bound = 3U;
        matrix m1(lower_bound,
                  //  index:  3  4  5  6  7  8
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(1);
    fifi::set_values(fifi::finite_field::binary8, coefficients.data(), {1});

    window = symbol_range{symbol_index{8U}, symbol_index{9U}};
    claimed = decoder.decode_symbol(decoder_storage[1].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{7}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{8}));

    {
        std::size_t lower_bound = 3U;
        matrix m1(lower_bound,
                  //  index:  3  4  5  6  7  8
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 1}}); // decoder_storage[1]

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(2);
    fifi::set_values(fifi::finite_field::binary8, coefficients.data(), {1, 1});

    window = symbol_range{symbol_index{7U}, symbol_index{9U}};
    claimed = decoder.decode_symbol(decoder_storage[2].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{7}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{8}));

    {
        std::size_t lower_bound = 3U;
        matrix m1(lower_bound,
                  //  index:  3  4  5  6  7  8
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 0},   // decoder_storage[2]
                   {0, 0, 0, 0, 0, 1}}); // decoder_storage[1]

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Next symbol
    coefficients.resize(6);
    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 1, 1, 1, 1, 1});

    window = symbol_range{symbol_index{3U}, symbol_index{9U}};
    claimed = decoder.decode_symbol(decoder_storage[4].data(), max_symbol_bytes,
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{7}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{8}));

    {
        std::size_t lower_bound = 3U;
        matrix m1(lower_bound,
                  //  index:  3  4  5  6  7  8
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[0]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[3]
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[4]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 0},   // decoder_storage[2]
                   {0, 0, 0, 0, 0, 1}}); // decoder_storage[1]

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }
}

TEST(detail_slide_test_stack_decoder, swap_decode_binary8)
{
    byte_count max_symbol_bytes{4U};
    symbol_count symbols{6U};

    // Create a config object
    binary8_stack::config config;
    config.max_symbol_bytes = max_symbol_bytes;
    // Create the decoder
    binary8_stack decoder;

    decoder.configure(config);

    // We need to provide the symbol storage for the decoder
    std::vector<std::vector<uint8_t>> decoder_storage(
        symbols.value, std::vector<uint8_t>(max_symbol_bytes.value));

    // Provide the decoder with the storage
    for (std::size_t i = 0; i < symbols.value; ++i)
    {
        decoder.push_symbol();
    }

    EXPECT_EQ(symbol_count{6U}, decoder.symbols_missing());
    EXPECT_EQ(symbol_count{0U}, decoder.symbols_partially_decoded());
    EXPECT_EQ(symbol_count{0U}, decoder.symbols_decoded());

    std::vector<uint8_t> coefficients;

    // Add a symbol
    coefficients.resize(5);
    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {0, 0, 1, 1, 1});

    symbol_range window = symbol_range{symbol_index{0U}, symbol_index{5U}};
    uint8_t* claimed = decoder.decode_symbol(
        decoder_storage[0].data(), byte_count{decoder_storage[0].size()},
        window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  //  index:  0  1  2  3  4  5
                  {{0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 1, 1, 1, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Add a decoded symbol
    claimed = decoder.decode_systematic_symbol(
        decoder_storage[1].data(), byte_count{decoder_storage[1].size()},
        symbol_index{2U});

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  // index:
                  // 0  1  2  3  4  5
                  {{0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 1, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 1, 1, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Check that we swapped the pointers
    EXPECT_EQ(decoder_storage[1].data(), decoder.symbol_data(symbol_index{2}));
    EXPECT_EQ(decoder_storage[0].data(), decoder.symbol_data(symbol_index{3}));

    claimed = decoder.decode_systematic_symbol(
        decoder_storage[2].data(), byte_count{decoder_storage[2].size()},
        symbol_index{0U});

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  // index:
                  // 0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[2]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 1, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 1, 1, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Add a new symbol
    coefficients.resize(6);
    fifi::set_values(fifi::finite_field::binary8, coefficients.data(),
                     {1, 1, 1, 1, 1, 1});

    window = symbol_range{symbol_index{0U}, symbol_index{6U}};
    claimed = decoder.decode_symbol(decoder_storage[3].data(),
                                    byte_count{decoder_storage[3].size()},
                                    window, coefficients.data());

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{0}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));

    {
        std::size_t lower_bound = 0U;
        matrix m1(lower_bound,
                  // index:
                  // 0  1  2  3  4  5
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[2]
                   {0, 1, 0, 0, 0, 1}, // decoder_storage[3]
                   {0, 0, 1, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 0, 1, 1, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // Move the stream
    claimed = decoder.pop_symbol();
    EXPECT_EQ(decoder_storage[2].data(), claimed);

    decoder.push_symbol();

    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));

    {
        std::size_t lower_bound = 1U;
        matrix m1(lower_bound,
                  // index:
                  // 1  2  3  4  5  6
                  {{1, 0, 0, 0, 1, 0}, // decoder_storage[3]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    claimed = decoder.decode_systematic_symbol(
        decoder_storage[2].data(), byte_count{decoder_storage[2].size()},
        symbol_index{1U});

    // The symbol is linear independent so we get nullptr back
    EXPECT_EQ(claimed, nullptr);

    // Check that we swapped the pointers
    EXPECT_EQ(decoder_storage[2].data(), decoder.symbol_data(symbol_index{1}));
    EXPECT_EQ(decoder_storage[3].data(), decoder.symbol_data(symbol_index{5}));

    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{1}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{2}));
    EXPECT_TRUE(decoder.is_symbol_partially_decoded(symbol_index{3}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{4}));
    EXPECT_TRUE(decoder.is_symbol_decoded(symbol_index{5}));
    EXPECT_TRUE(decoder.is_symbol_missing(symbol_index{6}));

    {
        std::size_t lower_bound = 1U;
        matrix m1(lower_bound,
                  // index:
                  // 1  2  3  4  5  6
                  {{1, 0, 0, 0, 0, 0}, // decoder_storage[2]
                   {0, 1, 0, 0, 0, 0}, // decoder_storage[1]
                   {0, 0, 1, 1, 0, 0}, // decoder_storage[0]
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 0}, // decoder_storage[3]
                   {0, 0, 0, 0, 0, 0}});

        matrix m2 = to_matrix(decoder);

        EXPECT_EQ(m1, m2);
    }

    // The symbol is linear dependent so we get it back
    claimed = decoder.decode_systematic_symbol(
        decoder_storage[4].data(), byte_count{decoder_storage[4].size()},
        symbol_index{1U});

    EXPECT_EQ(claimed, decoder_storage[4].data());
}
