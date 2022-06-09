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
#include <sstream>
#include <string>

#include <bourne/json.hpp>

#include "../log_callback.hpp"

#include "to_string.hpp"

#include "../version.hpp"

namespace kodo
{
inline namespace STEINWURF_KODO_VERSION
{
namespace detail
{

inline void to_object(bourne::json&)
{
}

template <class Key, class Value>
inline void to_object(bourne::json& json, Key&& key, Value&& value)
{
    json[key] = to_string(value);
}

template <class Key, class Value, class... Args>
inline void to_object(bourne::json& json, Key&& key, Value&& value,
                      Args&&... args)
{

    to_object(json, std::move(key), std::move(value));
    to_object(json, args...);
}

/// Logging layer
template <class Super>
class layer_log : public Super
{
public:
    template <class... Args>
    void log(const Args&... args) const
    {
        if (m_callback == nullptr)
        {
            return;
        }

        std::string result = "";
        int unpack[]{0, (result += to_string(args), 0)...};
        (void)unpack;
        m_callback(m_name, result, m_user_data);
    }

    template <class... Args>
    void log_json(const Args&... args) const
    {
        if (m_callback == nullptr)
        {
            return;
        }

        bourne::json object;
        to_object(object, args...);

        m_callback(m_name, object.dump(), m_user_data);
    }

    auto log_name() const -> std::string
    {
        return m_name;
    }

    void set_log_name(const std::string& name)
    {
        m_name = name;
    }

    void enable_log(const log_callback& callback, void* user_data)
    {
        m_callback = callback;
        m_user_data = user_data;
    }

    void disable_log()
    {
        m_callback = nullptr;
        m_user_data = nullptr;
    }

    bool is_log_enabled() const
    {
        return m_callback != nullptr;
    }

private:
    std::string m_name = "";
    log_callback m_callback = nullptr;
    void* m_user_data = nullptr;
};
}
}
}
