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

#include <fifi/math.hpp>

/// @example basic_arithmetic.cpp
///
/// An example to show how to perform basic arithmetics in finite fields.

void run_binary()
{
    printf("//-------------------------------------------------------------\n");
    printf("// binary field: GF(2)\n");
    printf("//-------------------------------------------------------------\n");

    auto math = fifi::math(fifi::finite_field::binary);
    uint32_t a = 0;
    uint32_t b = 1;

    printf(" max value: %d\n", math.max_value());
    printf(" %d + %d = %d\n", a, b, math.add(a, b));
    printf(" %d - %d = %d\n", a, b, math.subtract(a, b));
    printf(" %d * %d = %d\n", a, b, math.multiply(a, b));
    printf(" %d / %d = %d\n", a, b, math.divide(a, b));
    printf(" ~%d = %d\n", b, math.invert(b));

    printf("\n");
}

void run_binary4()
{
    printf("//-------------------------------------------------------------\n");
    printf("// binary4 field: GF(2^4)\n");
    printf("//-------------------------------------------------------------\n");

    auto math = fifi::math(fifi::finite_field::binary4);
    uint32_t a = 2;
    uint32_t b = 11;

    printf(" max value: %d\n", math.max_value());
    printf(" %d + %d = %d\n", a, b, math.add(a, b));
    printf(" %d - %d = %d\n", a, b, math.subtract(a, b));
    printf(" %d * %d = %d\n", a, b, math.multiply(a, b));
    printf(" %d / %d = %d\n", a, b, math.divide(a, b));
    printf(" ~%d = %d\n", b, math.invert(b));

    printf("\n");
}

void run_binary8()
{
    printf("//-------------------------------------------------------------\n");
    printf("// binary8 field: GF(2^8)\n");
    printf("//-------------------------------------------------------------\n");

    auto math = fifi::math(fifi::finite_field::binary8);
    uint32_t a = 22;
    uint32_t b = 101;

    printf(" max value: %d\n", math.max_value());
    printf(" %d + %d = %d\n", a, b, math.add(a, b));
    printf(" %d - %d = %d\n", a, b, math.subtract(a, b));
    printf(" %d * %d = %d\n", a, b, math.multiply(a, b));
    printf(" %d / %d = %d\n", a, b, math.divide(a, b));
    printf(" ~%d = %d\n", b, math.invert(b));

    printf("\n");
}

void run_binary16()
{
    printf("//-------------------------------------------------------------\n");
    printf("// binary8 field: GF(2^16)\n");
    printf("//-------------------------------------------------------------\n");

    auto math = fifi::math(fifi::finite_field::binary16);
    uint32_t a = 3423;
    uint32_t b = 41781;

    printf(" max value: %d\n", math.max_value());
    printf(" %d + %d = %d\n", a, b, math.add(a, b));
    printf(" %d - %d = %d\n", a, b, math.subtract(a, b));
    printf(" %d * %d = %d\n", a, b, math.multiply(a, b));
    printf(" %d / %d = %d\n", a, b, math.divide(a, b));
    printf(" ~%d = %d\n", b, math.invert(b));

    printf("\n");
}

void run_prime2325()
{
    printf("//-------------------------------------------------------------\n");
    printf("// prime2325 field: GF(2^32-5)\n");
    printf("//-------------------------------------------------------------\n");

    auto math = fifi::math(fifi::finite_field::prime2325);
    uint32_t a = 23354;
    uint32_t b = 781554;

    printf(" max value: %u\n", math.max_value());
    printf(" %d + %d = %u\n", a, b, math.add(a, b));
    printf(" %d - %d = %u\n", a, b, math.subtract(a, b));
    printf(" %d * %d = %u\n", a, b, math.multiply(a, b));
    printf(" %d / %d = %u\n", a, b, math.divide(a, b));
    printf(" ~%d = %u\n", b, math.invert(b));

    printf("\n");
}

int main()
{
    printf("Basic arithmetics in various finite fields:\n\n");

    run_binary();
    run_binary4();
    run_binary8();
    run_binary16();
    run_prime2325();
    unsigned long int i = (unsigned long int) -1;
    printf("%lu\n", i);
    return 0;
}
