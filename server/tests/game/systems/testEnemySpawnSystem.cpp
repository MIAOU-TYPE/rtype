/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testEnemySpawnSystem
*/

#include <gtest/gtest.h>
#include "game/systems/enemySpawn/EnemySpawnSystem.hpp"
#include "game/world/World.hpp"
#include "ecs/components/Health.hpp"

TEST(EnemySpawnSystem, spawns_enemy_every_2_seconds)
{
    Game::World world;
    auto &reg = world.registry();

    size_t before = reg.getComponents<Ecs::Health>().size();

    Game::EnemySpawnSystem::update(world, 2.1f);

    size_t after = reg.getComponents<Ecs::Health>().size();

    EXPECT_GT(after, before);
}
