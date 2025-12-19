/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AISystem
*/

#include <gtest/gtest.h>
#include "AIBrain.hpp"
#include "Velocity.hpp"
#include "AISystem.hpp"
#include "World.hpp"

TEST(AISystem, idle_moves_slowly)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::AIBrain>(e);
    reg.emplaceComponent<Ecs::Velocity>(e);
    reg.emplaceComponent<Ecs::Position>(e, 0.f, 0.f);
    reg.emplaceComponent<Ecs::Target>(e);

    auto &tgt = reg.getComponents<Ecs::Target>()[static_cast<size_t>(e)];
    tgt->targetId = SIZE_MAX;

    Game::AISystem::update(world, 0.1f);

    const auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];
    ASSERT_TRUE(vel.has_value());

    EXPECT_FLOAT_EQ(vel->vx, -20.f);
}

TEST(AISystem, transitions_to_patrol_after_1s)
{
    Game::World world;
    auto &reg = world.registry();

    const Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::AIBrain>(e);
    reg.emplaceComponent<Ecs::Velocity>(e);
    reg.emplaceComponent<Ecs::Position>(e, 0.f, 0.f);
    reg.emplaceComponent<Ecs::Target>(e);

    auto &tgt = reg.getComponents<Ecs::Target>()[static_cast<size_t>(e)];
    tgt->targetId = SIZE_MAX;

    Game::AISystem::update(world, 1.6f);

    const auto &brain = reg.getComponents<Ecs::AIBrain>()[static_cast<size_t>(e)];
    ASSERT_TRUE(brain.has_value());

    EXPECT_EQ(brain->state, Ecs::AIState::Patrol);
}
