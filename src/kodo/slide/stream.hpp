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
#include <deque>

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace slide
{
/// Stream container - implemented as an adapter around a std::deque.
///
/// The stream elements are pushed and popped from the stream in a FIFO order.
/// Each element has a specific absolute stream index.
///
/// Example:
///
///    Pushing to the stream will increase the size and popping will move the
///    offset.
///
///         stream<bool> example;
///
///         example.push(true); // size = 1, offset = 0
///         example.push(false); // size = 2, offset = 0
///         example.push(true); // size = 3, offset = 0
///         example.pop();       // size = 2, offset = 1
///         example.push(false); // size = 3, offset = 1
///
///    Element access uses absolute indexing.
///
///         bool b0 = example[0]; // ILLEGAL! The element at index 0 was popped
///         bool b1 = example[1]; // OK! The element at index 1
///         bool b2 = example[2]; // OK! The element at index 2
///         bool b3 = example[3]; // OK! The element at index 3
///
template <class Type>
class stream
{
public:
    /// @return The number of elements in the stream.
    std::size_t size() const
    {
        return m_stream.size();
    }

    /// @return True if the stream is empty, otherwise false.
    bool is_empty() const
    {
        return size() == 0;
    }

    /// @return This is the index of the "oldest" element in the FIFO stream.
    ///         I.e. to iterate over all elements in the stream:
    ///
    ///             stream<bool> values;
    ///
    ///             /// ... some code pushing and popping
    ///
    ///             std::size_t i = values.lower_bound();
    ///
    ///             while (i < values.upper_bound())
    ///             {
    ///                 std::cout << i << " " << values[i] << "\n";
    ///                 ++i;
    ///             }
    ///
    std::size_t lower_bound() const
    {
        return m_lower_bound;
    }

    /// @return The non-inclusive index of the newest symbol in the FIFO stream.
    ///         Non inclusive means that the newest symbol has index
    //          upper_bound() - 1. The reasons for making the range of stream
    ///         indicies non-inclusive are elegance you can find more
    ///         information on this by looking at the Python's range(...)
    ///         function is non inclusive.
    std::size_t upper_bound() const
    {
        return m_lower_bound + m_stream.size();
    }

    /// Set the stream lower bound. The stream must be empty before calling this
    /// function.
    /// @param lower_bound The new lower bound.
    void set_lower_bound(std::size_t lower_bound)
    {
        assert(is_empty());
        assert(lower_bound >= stream::lower_bound());
        m_lower_bound = lower_bound;
    }

    /// @param element Push element in the front of the FIFO stream.
    /// @return The stream index assigned to the element.
    std::size_t push(const Type& element)
    {
        m_stream.push_back(element);
        return m_lower_bound + (m_stream.size() - 1);
    }

    /// @param data Push element in the front of the FIFO stream.
    /// @return The stream index assigned to the element.
    std::size_t push(Type&& data)
    {
        m_stream.push_back(std::move(data));
        return m_lower_bound + (m_stream.size() - 1);
    }

    /// Pop the "oldest" element from the FIFO stream
    /// @return The stream index assigned to the popped element.
    std::size_t pop()
    {
        assert(m_stream.size() > 0U);

        m_stream.pop_front();
        return m_lower_bound++;
    }

    /// Reset the state of the stream
    void reset()
    {
        m_stream.resize(0U);
        m_lower_bound = 0U;
    }

    /// @return The element located at the back of the FIFO stream
    Type& front()
    {
        assert(size() > 0U);
        return at(lower_bound());
    }

    /// @return The element located at the back of the FIFO stream
    const Type& front() const
    {
        assert(size() > 0U);
        return at(lower_bound());
    }

    /// @param index Access the element in the stream with the corresponding
    ///        index.
    /// @return The element located at the index
    Type& at(std::size_t index)
    {
        assert(index >= m_lower_bound);
        assert(index < m_lower_bound + m_stream.size());

        // The front of the deque i.e. index 0 matches the front
        // of the stream
        std::size_t mapped_index = index - m_lower_bound;
        assert(mapped_index < m_stream.size());

        return m_stream.at((std::size_t)mapped_index);
    }

    /// @param index Access the element in the stream with the corresponding
    ///        index.
    /// @return The element located at the index
    const Type& at(std::size_t index) const
    {
        assert(index >= m_lower_bound);
        assert(index < m_lower_bound + m_stream.size());

        // The front of the deque i.e. index 0 matches the front
        // of the stream
        std::size_t mapped_index = index - m_lower_bound;
        assert(mapped_index < m_stream.size());

        return m_stream.at((std::size_t)mapped_index);
    }

    /// @param index Access the element in the stream with the corresponding
    ///        index.
    /// @return The element located at the index
    const Type& operator[](std::size_t index) const
    {
        return at(index);
    }

    /// @param index Access the element in the stream with the corresponding
    ///        index.
    /// @return The element located at the index
    Type& operator[](std::size_t index)
    {
        return at(index);
    }

    /// @param index Check if index is a valid index in the stream
    /// @return True if an element with index is in the stream otherwise false.
    bool in_stream(std::size_t index) const
    {
        if (index < lower_bound())
        {
            return false;
        }

        if (index >= upper_bound())
        {
            return false;
        }

        return true;
    }

    /// @return Iterator to the beginning of the stream
    friend auto begin(const stream& r)
    {
        return r.m_stream.begin();
    }

    /// @return Iterator to the beginning of the stream
    friend auto begin(stream& r)
    {
        return r.m_stream.begin();
    }

    /// @return Iterator to the end of the stream
    friend auto end(const stream& r)
    {
        return r.m_stream.end();
    }

    /// @return Iterator to the end of the stream
    friend auto end(stream& r)
    {
        return r.m_stream.end();
    }

private:
    // The index of the "oldest" element in the stream
    std::size_t m_lower_bound = 0;

    // Container for the elements in the stream
    std::deque<Type> m_stream;
};
}
}
}
