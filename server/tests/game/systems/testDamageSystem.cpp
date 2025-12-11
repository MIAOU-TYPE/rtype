/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testDamageSystem
*/

#include <gtest/gtest.h>
#include "ecs/components/Health.hpp"
#include "game/systems/damage/DamageSystem.hpp"
#include "game/world/World.hpp"

TEST(DamageSystem, removes_dead_entities)
{
    Game::World world;
    auto &reg = world.registry();

    Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::Health>(e, 0, 100);

    Game::DamageSystem::update(world);

    EXPECT_FALSE(reg.getComponents<Ecs::Health>()[static_cast<size_t>(e)].has_value());
}
