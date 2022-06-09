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

#include <kodo/detail/slide/layer_common_symbol_storage.hpp>

#include <kodo/detail/to_symbol_frame.hpp>
#include <kodo/detail/to_symbol_range.hpp>

#include <gtest/gtest.h>
#include <stub/function.hpp>

using namespace kodo::detail;

namespace
{
struct dummy
{
    struct config
    {
    };

    template <class Config>
    void configure(const Config&)
    {
    }

    auto to_symbol_range(symbol_index lower_bound, symbol_count symbols) const
        -> symbol_range
    {
        return kodo::detail::to_symbol_range(lower_bound, symbols);
    }

    symbol_frame to_symbol_frame(symbol_range range) const
    {
        return kodo::detail::to_symbol_frame(fifi::field::binary8{}, range);
    }
};
}

// In this test we check that pushing an popping symbols
// from the stream updates the indexes as it should
TEST(detail_slide_test_layer_common_symbol_storage, test_push_pop)
{
    slide::layer_common_symbol_storage<uint8_t*, dummy>::config config;
    slide::layer_common_symbol_storage<uint8_t*, dummy> stack;

    // A small note for how to read this unit test. We are not going to test
    // the stream_range(), stream_frame() and stream_memory() functions since
    // these are all derived values from a call to to_symbol_range(...). So
    // instead we will check that the right parameters are passed to the
    // to_symbol_range(...) function
    config.max_symbol_bytes = byte_count{20};
    stack.configure(config);

    EXPECT_EQ(stack.stream_lower_bound(), symbol_index{0});
    EXPECT_EQ(stack.stream_upper_bound(), symbol_index{0});

    stack.push_symbol(nullptr, byte_count{0});

    symbol_index lower_bound{0U};
    symbol_index upper_bound{1U};
    symbol_range range{lower_bound, upper_bound};
    symbol_frame frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{0U}));

    uint8_t* data = stack.pop_symbol();
    EXPECT_EQ(data, nullptr);

    stack.push_symbol(nullptr, byte_count{0});

    lower_bound = symbol_index{1U};
    upper_bound = symbol_index{2U};
    range = symbol_range{lower_bound, upper_bound};
    frame = symbol_frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{1U}));

    stack.set_symbol_storage(symbol_index{1U}, (uint8_t*)0xdeadbeef,
                             byte_count{10});

    EXPECT_TRUE(stack.is_symbol_initialized(symbol_index{1U}));
    EXPECT_EQ((uint8_t*)0xdeadbeef, stack.symbol_data(symbol_index{1U}));
    EXPECT_EQ(byte_count{10}, stack.symbol_bytes(symbol_index{1U}));

    data = stack.pop_symbol();
    EXPECT_EQ(data, (uint8_t*)0xdeadbeef);

    stack.push_symbol(nullptr, byte_count{0});

    lower_bound = symbol_index{2U};
    upper_bound = symbol_index{3U};
    range = symbol_range{lower_bound, upper_bound};
    frame = symbol_frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{2U}));

    stack.push_symbol(nullptr, byte_count{0});

    lower_bound = symbol_index{2U};
    upper_bound = symbol_index{4U};
    range = symbol_range{lower_bound, upper_bound};
    frame = symbol_frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{2U}));
    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{3U}));

    stack.set_symbol_storage(symbol_index{3U}, (uint8_t*)0xdeadbeef,
                             byte_count{10});

    EXPECT_FALSE(stack.is_symbol_initialized(symbol_index{2U}));
    EXPECT_TRUE(stack.is_symbol_initialized(symbol_index{3U}));

    data = stack.pop_symbol();
    EXPECT_EQ(data, nullptr);

    lower_bound = symbol_index{3U};
    upper_bound = symbol_index{4U};
    range = symbol_range{lower_bound, upper_bound};
    frame = symbol_frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
    EXPECT_TRUE(stack.is_symbol_initialized(symbol_index{3U}));

    data = stack.pop_symbol();
    EXPECT_EQ(data, (uint8_t*)0xdeadbeef);

    lower_bound = symbol_index{4U};
    upper_bound = symbol_index{4U};
    range = symbol_range{lower_bound, upper_bound};
    frame = symbol_frame{lower_bound, upper_bound};

    EXPECT_EQ(lower_bound, stack.stream_lower_bound());
    EXPECT_EQ(upper_bound, stack.stream_upper_bound());
    EXPECT_EQ(range, stack.stream_range());
    EXPECT_EQ(frame, stack.stream_frame());
}
