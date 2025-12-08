/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testMovementSystem
*/

#include <gtest/gtest.h>
#include "Position.hpp"
#include "Velocity.hpp"
#include "MovementSystem.hpp"
#include "World.hpp"

TEST(MovementSystem, moves_entities_correctly)
{
    Game::World world;
    Ecs::Entity e = world.createPlayer();

    auto &reg = world.registry();

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];
    auto &pos = reg.getComponents<Ecs::Position>()[static_cast<size_t>(e)];

    ASSERT_TRUE(vel.has_value());
    ASSERT_TRUE(pos.has_value());

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

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];
    auto &pos = reg.getComponents<Ecs::Position>()[static_cast<size_t>(e)];

    ASSERT_TRUE(vel.has_value());
    ASSERT_TRUE(pos.has_value());

    vel->vx = 100.f;
    vel->vy = 0.f;

    Game::MovementSystem::update(world, 0.5f);

    EXPECT_FLOAT_EQ(pos->x, 100.f + 100.f * 0.5f);
    EXPECT_FLOAT_EQ(pos->y, 100.f);
}
