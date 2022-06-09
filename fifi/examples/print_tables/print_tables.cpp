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

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <fifi/math.hpp>

/// @example print_tables.cpp
///
/// Prints the multiplication and division tables for a finite field.
void print_table(fifi::finite_field field, bool multiply)
{
    auto math = fifi::math(field);

    printf("{\n");

    for (uint32_t i = 0; i <= math.max_value(); ++i)
    {
        printf("\n    ");
        for (uint32_t j = 0; j <= math.max_value(); ++j)
        {
            if (multiply)
            {
                printf("%3d", math.multiply(i, j));
            }
            else
            {
                // Cannot divide by zero
                printf("%3d", (j != 0) ? math.divide(i, j) : 0);
            }

            // Skip postfix for the last element in the printed array
            if (!(i == j && j == math.max_value()))
                printf(", ");
        }
    }

    printf("\n}\n");
}

int main()
{
    print_table(fifi::finite_field::binary4, true);
    print_table(fifi::finite_field::binary4, false);

    return 0;
}
