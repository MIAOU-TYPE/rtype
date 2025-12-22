/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testMovementSystem
*/

#include <gtest/gtest.h>
#include "MovementSystem.hpp"
#include "Position.hpp"
#include "Velocity.hpp"
#include "World.hpp"

TEST(MovementSystem, moves_entities_correctly)
{
    Game::World world;
    Ecs::Entity e = world.createPlayer();

    auto &reg = world.registry();

    auto &vel = reg.getComponents<Ecs::Velocity>().at(static_cast<size_t>(e));
    const auto &pos = reg.getComponents<Ecs::Position>().at(static_cast<size_t>(e));

    ASSERT_TRUE(vel.has_value());
    ASSERT_TRUE(pos.has_value());

    if (!vel)
        ASSERT_TRUE(vel);
    vel->vx = 10.f;
    vel->vy = 20.f;

    Game::MovementSystem::update(world, 1.f);

    EXPECT_FLOAT_EQ(pos->x, 100.f + 10.f);
    EXPECT_FLOAT_EQ(pos->y, 100.f + 20.f);
}

TEST(MovementSystem, dt_affects_speed)
{
    Game::World world;
    Ecs::Entity e = world.createPlayer();

    auto &reg = world.registry();

    auto &vel = reg.getComponents<Ecs::Velocity>().at(static_cast<size_t>(e));
    auto &pos = reg.getComponents<Ecs::Position>().at(static_cast<size_t>(e));

    ASSERT_TRUE(vel.has_value());
    ASSERT_TRUE(pos.has_value());

    vel->vx = 100.f;
    vel->vy = 0.f;

    Game::MovementSystem::update(world, 0.5f);

    EXPECT_FLOAT_EQ(pos->x, 100.f + 100.f * 0.5f);
    EXPECT_FLOAT_EQ(pos->y, 100.f);
}
