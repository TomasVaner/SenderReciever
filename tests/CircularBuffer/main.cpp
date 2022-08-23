#include "../../libs/circular_buffer.h"
#include <gtest/gtest.h>

TEST(CircularBufferTests, PopTest)
{
    support::CircularBuffer<int> buffer(6);
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    for (int i = 0; i < 5; ++i)
    {
        auto val = buffer.Pop();
        EXPECT_TRUE(val.has_value()) << "Buffer is empty!";
        if (val.has_value())
        EXPECT_EQ(val.value(), i) << "Wrong value [" << i << "] " << val.value();
    }
}

TEST(CircularBufferTests, FullTest)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        EXPECT_TRUE(buffer.Push(i)) << "Buffer was full when adding items";
    EXPECT_FALSE(buffer.Push(10)) << "Adding item after filling did not return 'true'";
    EXPECT_FALSE(buffer.IsEmpty()) << "Buffer is empty after filling";
    EXPECT_TRUE(buffer.IsFull()) << "Buffer is not full after filling";
    auto val = buffer.Pop();
    EXPECT_TRUE(val.has_value()) << "Pop 0 did not return a value";
    EXPECT_FALSE(buffer.IsEmpty()) << "Buffer is empty after pop";
    EXPECT_FALSE(buffer.IsFull()) << "Buffer is full after pop";
    for (int i = 0; i < 4; ++i)
    {
        val = buffer.Pop();
        EXPECT_TRUE(val.has_value()) << "Pop " << i + 1 << " did not return a value";
    }
    EXPECT_TRUE(buffer.IsEmpty()) << "Buffer is not empty after emptying";
    EXPECT_FALSE(buffer.IsFull()) << "Buffer is full after emptying";
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}