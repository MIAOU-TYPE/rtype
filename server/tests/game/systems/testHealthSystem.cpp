/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testHealthSystem
*/

#include <gtest/gtest.h>
#include "ecs/components/Health.hpp"
#include "game/systems/health/HealthSystem.hpp"
#include "game/world/World.hpp"

TEST(HealthSystem, destroys_entity_at_zero_hp)
{
    Game::World world;
    Ecs::Entity e = world.createPlayer();

    auto &reg = world.registry();
    auto &hp = reg.getComponents<Ecs::Health>()[static_cast<size_t>(e)];

    ASSERT_TRUE(hp.has_value());

    hp->hp = 0;

    Game::HealthSystem::update(world);

    EXPECT_FALSE(reg.getComponents<Ecs::Health>()[static_cast<size_t>(e)].has_value());
}
