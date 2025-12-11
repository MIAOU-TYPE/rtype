/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AISystem
*/

#include <gtest/gtest.h>
#include "ecs/components/AIBrain.hpp"
#include "ecs/components/Velocity.hpp"
#include "game/systems/AI/AISystem.hpp"
#include "game/world/World.hpp"

TEST(AISystem, idle_moves_slowly)
{
    Game::World world;
    auto &reg = world.registry();

    Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::AIBrain>(e);
    reg.emplaceComponent<Ecs::Velocity>(e);

    Game::AISystem::update(world, 0.1f);

    auto &vel = reg.getComponents<Ecs::Velocity>()[static_cast<size_t>(e)];
    ASSERT_TRUE(vel.has_value());

    EXPECT_FLOAT_EQ(vel->vx, -20.f);
}

TEST(AISystem, transitions_to_patrol_after_1s)
{
    Game::World world;
    auto &reg = world.registry();

    Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::AIBrain>(e);
    reg.emplaceComponent<Ecs::Velocity>(e);

    Game::AISystem::update(world, 1.1f);

    auto &brain = reg.getComponents<Ecs::AIBrain>()[static_cast<size_t>(e)];
    ASSERT_TRUE(brain.has_value());

    EXPECT_EQ(brain->state, Ecs::AIState::Patrol);
}
