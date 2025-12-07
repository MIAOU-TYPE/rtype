/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testBuffer
*/

#include <gtest/gtest.h>
#include "RingBuffer/RingBuffer.hpp"

using namespace Buffer;

TEST(RingBufferTests, EmptyOnCreation)
{
    RingBuffer<int> rb(3);

    EXPECT_TRUE(rb.isEmpty());
    EXPECT_FALSE(rb.isFull());
}

TEST(RingBufferTests, PushUntilFull)
{
    RingBuffer<int> rb(3);

    EXPECT_TRUE(rb.push(1));
    EXPECT_TRUE(rb.push(2));
    EXPECT_TRUE(rb.push(3));

    EXPECT_TRUE(rb.isFull());
    EXPECT_FALSE(rb.isEmpty());
    EXPECT_FALSE(rb.push(4));
}

TEST(RingBufferTests, PopUntilEmpty)
{
    RingBuffer<int> rb(3);
    int value = 0;

    rb.push(10);
    rb.push(20);

    EXPECT_FALSE(rb.isEmpty());
    EXPECT_FALSE(rb.isFull());

    EXPECT_TRUE(rb.pop(value));
    EXPECT_EQ(value, 10);

    EXPECT_TRUE(rb.pop(value));
    EXPECT_EQ(value, 20);

    EXPECT_TRUE(rb.isEmpty());
    EXPECT_FALSE(rb.pop(value));
}

TEST(RingBufferTests, TopReturnsFrontElement)
{
    RingBuffer<int> rb(3);

    rb.push(5);
    rb.push(10);

    EXPECT_EQ(rb.top(), 5);
}

TEST(RingBufferTests, TopThrowsOnEmpty)
{
    RingBuffer<int> rb(2);

    EXPECT_THROW(rb.top(), BufferError);
}

TEST(RingBufferTests, ClearResetsBuffer)
{
    RingBuffer<int> rb(3);
    rb.push(1);
    rb.push(2);

    rb.clear();

    EXPECT_TRUE(rb.isEmpty());
    EXPECT_FALSE(rb.isFull());

    int value;
    EXPECT_FALSE(rb.pop(value));
}

TEST(RingBufferTests, RingBehaviorWriteWrapsCorrectly)
{
    RingBuffer<int> rb(3);

    rb.push(1);
    rb.push(2);
    rb.push(3);

    int value;
    rb.pop(value);
    EXPECT_EQ(value, 1);

    EXPECT_TRUE(rb.push(4));
    EXPECT_TRUE(rb.isFull());

    rb.pop(value);
    EXPECT_EQ(value, 2);

    rb.pop(value);
    EXPECT_EQ(value, 3);

    rb.pop(value);
    EXPECT_EQ(value, 4);
}

TEST(RingBufferTests, PushPopAlternating)
{
    RingBuffer<int> rb(2);
    int value;

    EXPECT_TRUE(rb.push(10));
    EXPECT_TRUE(rb.pop(value));
    EXPECT_EQ(value, 10);

    EXPECT_TRUE(rb.push(20));
    EXPECT_TRUE(rb.push(30));
    EXPECT_TRUE(rb.isFull());
    EXPECT_FALSE(rb.push(40));

    EXPECT_FALSE(rb.isEmpty());
    EXPECT_TRUE(rb.pop(value));
    EXPECT_EQ(value, 20);

    EXPECT_TRUE(rb.pop(value));
    EXPECT_EQ(value, 30);

    EXPECT_TRUE(rb.isEmpty());
}
