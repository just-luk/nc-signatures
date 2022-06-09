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
#include <iomanip>
#include <list>
#include <sstream>

#include "../in_frame.hpp"
#include "../symbol_frame.hpp"

#include "../../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{
namespace slide
{
/// Layer for tracing coefficient storage information.
template <class Super>
struct layer_log_coefficients_storage : public Super
{

    /// Push symbol to the stream
    void push_symbol()
    {
        if (Super::is_log_enabled())
        {
            log_push_symbol();
        }
        else
        {
            Super::push_symbol();
        }
    }

    /// Set the coefficient vector
    void set_coefficient_vector(symbol_index index,
                                const coefficients_vector& the_vector)
    {
        if (Super::is_log_enabled())
        {
            log_set_coefficient_vector(index, the_vector);
        }
        else
        {
            Super::set_coefficient_vector(index, the_vector);
        }
    }

    /// Log pushing symbol to the stream
    void log_push_symbol()
    {
        std::stringstream stream;

        stream << "before push_symbol:\n";
        print_status(stream);

        Super::push_symbol();

        stream << "after push_symbol:\n";
        print_status(stream);

        Super::log("log_coefficient_storage_layer", stream.str());
    }

    /// Log setting the coefficient vector
    void log_set_coefficient_vector(symbol_index index,
                                    const coefficients_vector& the_vector)
    {
        std::stringstream stream;
        stream << "before set_coefficient_vector:\n";
        print_status(stream);

        stream << "index=" << index.value << "\n";
        Super::print_vector(stream, the_vector);
        stream << "\n";

        Super::set_coefficient_vector(index, the_vector);

        stream << "after set_coefficient_vector:\n";
        print_status(stream);

        Super::log("log_coefficient_storage_layer", stream.str());
    }

    /// Print the status
    void print_status(std::ostream& out)
    {
        symbol_frame stream_frame = Super::stream_frame();

        out << "index:  ";
        for (symbol_index i : stream_frame)
        {
            out << std::setw(3) << i.value << " ";
        }

        out << "\n";

        out << "status: ";
        for (symbol_index i : stream_frame)
        {
            if (Super::has_vector(i))
            {
                out << std::setw(3) << "  + ";
            }
            else
            {
                out << std::setw(3) << "  - ";
            }
        }

        out << "\n";
    }

    /// Print the coefficient storage
    void print_coefficient_storage(std::ostream& out)
    {
        auto range = Super::stream_range();

        // Make 6 spaces
        out << "      ";

        for (symbol_index i : range)
        {
            out << std::setw(3) << i.value << " ";
        }

        out << "\n";

        // Print the coefficients
        for (symbol_index i : range)
        {
            out << std::setw(3) << i.value << ": [";

            if (Super::has_vector(i))
            {
                auto view = Super::coefficients_data(i);
                auto frame = Super::coefficients_view_to_symbol_frame(view);

                for (symbol_index j : range)
                {
                    if (in_frame(frame, j))
                    {
                        auto value = Super::get_value(view, j);
                        out << std::setw(3) << value << " ";
                    }
                    else
                    {
                        out << "  x ";
                    }
                }
            }
            else
            {
                for (symbol_index j : range)
                {
                    (void)j;
                    out << "  x ";
                }
            }

            out << "]\n";
        }
    }
};
}
}
}
}
