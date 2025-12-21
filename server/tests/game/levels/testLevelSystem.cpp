/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testLevelSystem
*/

#include <gtest/gtest.h>
#include "LevelManager.hpp"
#include "LevelSystem.hpp"
#include "MockWorld.hpp"

TEST(LevelSystem, SpawnOneWave)
{
    MockWorld world;
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "L1",
      "duration": 20,
      "enemies": {
        "small": {
          "hp": 10,
          "speed": -100,
          "size": { "w": 8, "h": 8 },
          "sprite": "enemy.png"
        }
      },
      "waves": [
        { "time": 0.5, "enemies": { "small": 2 } }
      ]
    })";

    ASSERT_TRUE(mgr.load(json));

    EXPECT_EQ(world.registry().getComponents<Ecs::Position>().size(), 0u);

    // advance time twice → wave triggers
    Game::LevelSystem::update(world, mgr, 0.3f);
    Game::LevelSystem::update(world, mgr, 0.3f);

    auto &posArr = world.registry().getComponents<Ecs::Position>();

    int spawnedCount = 0;
    for (size_t i = 0; i < posArr.size(); ++i) {
        if (posArr.at(i).has_value())
            spawnedCount++;
    }

    EXPECT_EQ(spawnedCount, 2);

    for (size_t i = 0; i < posArr.size(); ++i) {
        if (!posArr.at(i).has_value())
            continue;

        EXPECT_TRUE(world.registry().hasComponent<Ecs::Velocity>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::Collision>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::Health>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::AIBrain>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::Damageable>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::Attack>(Ecs::Entity(i)));
        EXPECT_TRUE(world.registry().hasComponent<Ecs::Drawable>(Ecs::Entity(i)));
    }
}

TEST(LevelSystem, WaveTriggersOnlyOnce)
{
    MockWorld world;
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "L1",
      "duration": 20,
      "enemies": {
        "small": {
          "hp": 5,
          "speed": -100,
          "size": { "w": 5, "h": 5 },
          "sprite": ""
        }
      },
      "waves": [
        { "time": 1.0, "enemies": { "small": 1 } }
      ]
    })";

    ASSERT_TRUE(mgr.load(json));

    Game::LevelSystem::update(world, mgr, 1.2f);

    auto &posArr = world.registry().getComponents<Ecs::Position>();
    int firstSpawnCount = 0;
    for (size_t i = 0; i < posArr.size(); ++i)
        if (posArr.at(i).has_value())
            firstSpawnCount++;

    Game::LevelSystem::update(world, mgr, 5.f);

    int secondSpawnCount = 0;
    for (size_t i = 0; i < posArr.size(); ++i)
        if (posArr.at(i).has_value())
            secondSpawnCount++;

    EXPECT_EQ(firstSpawnCount, secondSpawnCount);
}
