/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testLifetimeSystem
*/

#include <gtest/gtest.h>
#include "Lifetime.hpp"
#include "LifetimeSystem.hpp"
#include "World.hpp"

TEST(LifetimeSystem, destroys_entity_after_time_expires)
{
    Game::World world;
    auto &reg = world.registry();

    Ecs::Entity e = reg.createEntity();
    reg.emplaceComponent<Ecs::Lifetime>(e, 0.5f);

    Game::LifetimeSystem::update(world, 0.6f);

    EXPECT_FALSE(reg.getComponents<Ecs::Lifetime>()[static_cast<size_t>(e)].has_value());
}
