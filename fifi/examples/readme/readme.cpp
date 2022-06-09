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

#include <iostream>

#include <fifi/math.hpp>

int main()
{
    // Print the version of the library used
    std::cout << "fifi version = " << fifi::version() << "\n";

    // Create the math object with a specific finite field
    fifi::math math(fifi::finite_field::binary8);

    uint8_t a = 10;
    uint8_t b = 128;

    // Multiply some values - check the API of math to see all the
    // available methods. We cast `a` and `b` to ints as std::cout will
    // interpret uint8_t as chars and therefor print them as characters
    // instead of numbers.
    std::cout << (int)a << " * " << (int)b << " = " << math.multiply(a, b)
              << "\n";

    return 0;
}
