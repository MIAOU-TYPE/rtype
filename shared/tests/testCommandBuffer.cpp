/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testCommandBuffer
*/

#include <atomic>
#include <gtest/gtest.h>
#include <thread>
#include "CommandBuffer.hpp"

using namespace Command;

struct TestCmd {
    int value;
};

struct HeavyCmd {
    std::string name;
    std::vector<int> data;
};

TEST(CommandBuffer, buffer_is_empty_initially)
{
    CommandBuffer<TestCmd> cb;
    EXPECT_TRUE(cb.empty());
}

TEST(CommandBuffer, push_makes_buffer_non_empty)
{
    CommandBuffer<TestCmd> cb;
    cb.push({42});
    EXPECT_FALSE(cb.empty());
}

TEST(CommandBuffer, pop_returns_elements_in_FIFO_order)
{
    CommandBuffer<TestCmd> cb;
    cb.push({1});
    cb.push({2});
    cb.push({3});

    TestCmd out;

    ASSERT_TRUE(cb.pop(out));
    EXPECT_EQ(out.value, 1);

    ASSERT_TRUE(cb.pop(out));
    EXPECT_EQ(out.value, 2);

    ASSERT_TRUE(cb.pop(out));
    EXPECT_EQ(out.value, 3);

    EXPECT_TRUE(cb.empty());
}

TEST(CommandBuffer, pop_on_empty_returns_false)
{
    CommandBuffer<TestCmd> cb;
    TestCmd out;
    EXPECT_FALSE(cb.pop(out));
}

TEST(CommandBuffer, clear_empties_buffer)
{
    CommandBuffer<TestCmd> cb;
    cb.push({10});
    cb.push({20});
    cb.push({30});

    cb.clear();

    EXPECT_TRUE(cb.empty());

    TestCmd out;
    EXPECT_FALSE(cb.pop(out));
}

TEST(CommandBuffer, handles_heavy_move_only_objects)
{
    CommandBuffer<HeavyCmd> cb;

    HeavyCmd cmd;
    cmd.name = "hello";
    cmd.data = {1, 2, 3, 4};

    cb.push(cmd);

    HeavyCmd out;
    ASSERT_TRUE(cb.pop(out));

    EXPECT_EQ(out.name, "hello");
    EXPECT_EQ(out.data.size(), 4);
    EXPECT_EQ(out.data[2], 3);
}

TEST(CommandBuffer, concurrent_push_and_pop_does_not_crash)
{
    CommandBuffer<TestCmd> cb;
    std::atomic<bool> running = true;

    std::thread producer([&]() {
        for (int i = 0; i < 200000; i++) {
            cb.push({i});
        }
        running = false;
    });

    TestCmd out;
    while (running || !cb.empty()) {
        cb.pop(out);
    }

    producer.join();
    SUCCEED();
}
