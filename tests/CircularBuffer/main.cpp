#include "../../libs/circular_buffer.h"
#include <gtest/gtest.h>

TEST(CircularBufferTests, PopTest)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    for (int i = 0; i < 5; ++i)
    {
        auto val = buffer.Pop();
        ASSERT_TRUE(val.has_value());
        ASSERT_EQ(val, i);
    }
}