#include "../../libs/circular_buffer.h"
#include <gtest/gtest.h>

TEST(CircularBufferTests, Push)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        EXPECT_TRUE(buffer.Push(i)) << "Buffer was full when adding items";
}

TEST(CircularBufferTests, PushAfterFilling)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    ASSERT_FALSE(buffer.Push(0));
}

TEST(CircularBufferTests, Pop)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    for (int i = 0; i < 5; ++i)
    {
        auto val = buffer.Pop();
        ASSERT_TRUE(val.has_value()) << "Did not get the value [" << i << "]";
        if (val.has_value())
        ASSERT_EQ(val.value(), i) << "Wrong value [" << i << "] " << val.value();
    }
}

TEST(CircularBufferTests, PopEmpty)
{
    support::CircularBuffer<int> buffer(5);
    auto val = buffer.Pop();
    ASSERT_FALSE(val.has_value()) << "Popped the value from empty buffer";
}

TEST(CircularBufferTests, Full)
{
    support::CircularBuffer<int> buffer(5);
    EXPECT_FALSE(buffer.IsFull());
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    ASSERT_TRUE(buffer.IsFull()) << "Buffer is not full";
}

TEST(CircularBufferTests, FullTwoSteps)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 3; ++i)
        EXPECT_TRUE(buffer.Push(i)) << "Buffer was full when adding items";
    EXPECT_FALSE(buffer.IsFull());
    for (int i = 0; i < 2; ++i)
        EXPECT_TRUE(buffer.Push(i)) << "Buffer was full when adding items";
    ASSERT_TRUE(buffer.IsFull());
}

TEST(CircularBufferTests, Empty)
{
    support::CircularBuffer<int> buffer(5);
    EXPECT_TRUE(buffer.IsEmpty()) << "Buffer is not empty";
    buffer.Push(0);
    ASSERT_FALSE(buffer.IsEmpty());
}

TEST(CircularBufferTests, EmptyAfterEmptying)
{
    support::CircularBuffer<int> buffer(5);
    buffer.Push(0);
    buffer.Push(0);
    buffer.Pop();
    buffer.Pop();
    ASSERT_TRUE(buffer.IsEmpty()) << "Buffer is not empty";
}

TEST(CircularBufferTests, SizePush)
{
    support::CircularBuffer<int> buffer(5);
    ASSERT_EQ(buffer.Size(), 0);
    buffer.Push(0);
    ASSERT_EQ(buffer.Size(), 1);
    buffer.Push(1);
    ASSERT_EQ(buffer.Size(), 2);
    buffer.Push(2);
    ASSERT_EQ(buffer.Size(), 3);
    buffer.Push(3);
    ASSERT_EQ(buffer.Size(), 4);
    buffer.Push(4);
    ASSERT_EQ(buffer.Size(), 5);
    buffer.Push(5);
    ASSERT_EQ(buffer.Size(), 5) << "Wrong size after pushing 6th element";
}

TEST(CircularBufferTests, SizePop)
{
    support::CircularBuffer<int> buffer(5);
    for (int i = 0; i < 5; ++i)
        buffer.Push(i);
    ASSERT_EQ(buffer.Size(), 5);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 4);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 3);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 2);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 1);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 0);
    buffer.Pop();
    ASSERT_EQ(buffer.Size(), 0);
}

TEST(CircularBufferTests, Capacity)
{
    support::CircularBuffer<int> buffer1(5);
    ASSERT_EQ(buffer1.Capacity(), static_cast<size_t>(5));
    support::CircularBuffer<int> buffer2(1024);
    ASSERT_EQ(buffer1.Capacity(), static_cast<size_t>(1024));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}