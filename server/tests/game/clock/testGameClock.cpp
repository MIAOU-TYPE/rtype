/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testGameClock
*/

#include <gtest/gtest.h>
#include <thread>
#include "GameClock.hpp"

TEST(GameClock, RestartReturnsPositiveDelta)
{
    Game::GameClock clock;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    double dt = clock.restart();

    EXPECT_GT(dt, 0.0);
}

TEST(GameClock, ElapsedWorksCorrectly)
{
    Game::GameClock clock;
    std::this_thread::sleep_for(std::chrono::milliseconds(15));

    double t = clock.elapsed();
    EXPECT_GT(t, 0.0);

    double t2 = clock.elapsed();
    EXPECT_GE(t2, t);
}

TEST(GameClock, RestartThenElapsedStartsAtZero)
{
    Game::GameClock clock;

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    clock.restart();

    double t = clock.elapsed();
    EXPECT_NEAR(t, 0.0, 0.002);
}
