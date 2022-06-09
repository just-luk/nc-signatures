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

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{
/// Small helper to control the amount of redundancy generated.
///
/// Using the common n,k termionology. For each k symbols we send n. This
/// means that r=n-k is the redundancy/repair symbols.
///
/// Example: n=6, k=4, r=2. We have k=4 source symbols, we send n=6 symbols
///          so after the 4'th source symbol we send r=2 (coded) redundancy
///          symbols.
///
/// Usage:
///
///     rate_controller rate = {6, 4};
///
///     // For every 6 symbols we send 4 are source symbols and 2 are
///     // repair.
///     assert(rate.send_repair() == false);
///     rate.advance();
///
///     assert(rate.send_repair() == false);
///     rate.advance();
///
///     assert(rate.send_repair() == false);
///     rate.advance();
///
///     assert(rate.send_repair() == false);
///     rate.advance();
///
///     assert(rate.send_repair() == true);
///     rate.advance();
///
///     assert(rate.send_repair() == true);
///     rate.advance();
///
///     // We now start over
///     assert(rate.send_repair() == false);
///     rate.advance();
///
class rate_controller
{
public:
    /// Create a new rate controller.
    /// Using the common n,k termionology. For each k symbols we send n.
    /// This means that r = n-k is the redundancy/repair symbols.
    /// @param n The n value.
    /// @param k The k value.
    rate_controller(std::size_t n, std::size_t k) : m_n(n), m_k(k)
    {
        assert(m_n > 0);
        assert(m_k > 0);
        assert(m_n >= m_k);
    }

    /// Increment position to be a number from 0 to n
    void advance()
    {
        assert(m_position < m_n);

        m_position = (m_position + 1) % m_n;

        assert(m_position < m_n);
    }

    /// If true we should generate a repair symbol. Otherwise send a source
    /// symbol.
    bool send_repair() const
    {
        return m_position >= m_k;
    }

private:
    /// The total number of symbols to send
    std::size_t m_n = 0;

    /// The number of source symbols
    std::size_t m_k = 0;

    /// The current position
    std::size_t m_position = 0;
};
}
}
}
