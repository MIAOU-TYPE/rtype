/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testTargetingSystem
*/

#include <gtest/gtest.h>

#include "TargetingSystem.hpp"
#include "World.hpp"

#include "Controllable.hpp"
#include "Position.hpp"
#include "Target.hpp"

using namespace Game;

static Ecs::Entity createPlayer(World &world, float x, float y)
{
    auto &reg = world.registry();
    const Ecs::Entity player = reg.createEntity();

    reg.emplaceComponent<Ecs::Position>(player, x, y);
    reg.emplaceComponent<Ecs::Controllable>(player, 0);

    return player;
}

static Ecs::Entity createEnemy(World &world, float x, float y, float vision)
{
    auto &reg = world.registry();
    const Ecs::Entity enemy = reg.createEntity();

    reg.emplaceComponent<Ecs::Position>(enemy, x, y);
    reg.emplaceComponent<Ecs::Target>(enemy, Ecs::Target{SIZE_MAX, vision});

    return enemy;
}

TEST(TargetingSystem, no_player_no_target_assigned)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity enemy = createEnemy(world, 0.f, 0.f, 200.f);

    TargetingSystem::update(world);

    const auto &tgt = *reg.getComponents<Ecs::Target>()[static_cast<size_t>(enemy)];
    EXPECT_EQ(tgt.targetId, SIZE_MAX);
}

TEST(TargetingSystem, assigns_target_when_player_in_range)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity player = createPlayer(world, 100.f, 0.f);
    const Ecs::Entity enemy = createEnemy(world, 0.f, 0.f, 150.f);

    TargetingSystem::update(world);

    const auto &tgt = *reg.getComponents<Ecs::Target>()[static_cast<size_t>(enemy)];
    EXPECT_EQ(tgt.targetId, static_cast<size_t>(player));
}

TEST(TargetingSystem, does_not_assign_target_when_player_out_of_range)
{
    World world;
    auto &reg = world.registry();

    createPlayer(world, 300.f, 0.f);
    const Ecs::Entity enemy = createEnemy(world, 0.f, 0.f, 100.f);

    TargetingSystem::update(world);

    const auto &tgt = *reg.getComponents<Ecs::Target>()[static_cast<size_t>(enemy)];
    EXPECT_EQ(tgt.targetId, SIZE_MAX);
}

TEST(TargetingSystem, clears_target_when_player_leaves_range)
{
    World world;
    auto &reg = world.registry();

    const Ecs::Entity player = createPlayer(world, 50.f, 0.f);
    const Ecs::Entity enemy = createEnemy(world, 0.f, 0.f, 100.f);

    TargetingSystem::update(world);
    EXPECT_EQ(reg.getComponents<Ecs::Target>()[static_cast<size_t>(enemy)]->targetId, static_cast<size_t>(player));

    reg.getComponents<Ecs::Position>()[static_cast<size_t>(player)]->x = 300.f;

    TargetingSystem::update(world);
    EXPECT_EQ(reg.getComponents<Ecs::Target>()[static_cast<size_t>(enemy)]->targetId, SIZE_MAX);
}
