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
#include <vector>

#include <fifi/math.hpp>

int main()
{
    std::cout << "SIMD accelerations" << std::endl;

    std::vector<uint8_t> x(32 + 16);
    std::vector<uint8_t> y(32 + 16);

    fifi::math m(fifi::finite_field::binary8);

    m.vector_add_into(x.data(), y.data(), x.size());

    std::cout << m.metrics_to_json() << std::endl;

    return 0;
}
