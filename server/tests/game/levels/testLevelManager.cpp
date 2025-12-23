/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testLevelManager
*/

#include <gtest/gtest.h>
#include "LevelManager.hpp"

TEST(LevelManager, Load_ValidJSON)
{
    const std::string jsonLevel = R"(
    {
      "name": "TestLevel",
      "duration": 30,
      "enemies": {
        "small": {
          "hp": 10,
          "speed": -50,
          "size": { "w": 10, "h": 10 },
          "spriteId": 19
        }
      },
      "waves": [
        { "time": 1.5, "enemies": { "small": 3 } }
      ]
    })";

    Game::LevelManager mgr;
    ASSERT_TRUE(mgr.load(jsonLevel));

    const Game::Level &lvl = mgr.getCurrentLevel();

    EXPECT_EQ(lvl.name, "TestLevel");
    EXPECT_FLOAT_EQ(lvl.duration, 30.f);

    ASSERT_EQ(lvl.enemyTypes.size(), 1u);
    ASSERT_TRUE(lvl.enemyTypes.contains("small"));

    const auto &e = lvl.enemyTypes.at("small");
    EXPECT_EQ(e.hp, 10);
    EXPECT_FLOAT_EQ(e.speed, -50.f);
    EXPECT_FLOAT_EQ(e.colW, 10.f);
    EXPECT_FLOAT_EQ(e.colH, 10.f);
    EXPECT_EQ(e.sprite, 19);

    ASSERT_EQ(lvl.waves.size(), 1u);
    EXPECT_FLOAT_EQ(lvl.waves[0].time, 1.5f);

    ASSERT_EQ(lvl.waves[0].groups.size(), 1u);
    EXPECT_EQ(lvl.waves[0].groups[0].type, "small");
    EXPECT_EQ(lvl.waves[0].groups[0].count, 3);
}

TEST(LevelManager, Load_InvalidJSON_SyntaxError)
{
    Game::LevelManager mgr;
    const std::string broken = R"({ "name": "Bad", BAD )";
    EXPECT_FALSE(mgr.load(broken));
}

TEST(LevelManager, Load_InvalidJSON_NoWaves)
{
    Game::LevelManager mgr;

    const std::string invalid = R"(
    {
      "name": "InvalidLevel",
      "duration": 10
    })";

    EXPECT_FALSE(mgr.load(invalid));
}

TEST(LevelManager, TimeAdvancement)
{
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "TimeTest",
      "enemies": {
        "dummy": {
          "hp": 1,
          "speed": -1,
          "size": { "w": 1, "h": 1 }
        }
      },
      "waves": [
        { "time": 0, "enemies": { "dummy": 1 } }
      ]
    })";

    ASSERT_TRUE(mgr.load(json));

    EXPECT_FLOAT_EQ(mgr.getTime(), 0.f);

    mgr.advance(0.5f);
    EXPECT_FLOAT_EQ(mgr.getTime(), 0.5f);

    mgr.reset();
    EXPECT_FLOAT_EQ(mgr.getTime(), 0.f);
}

TEST(LevelManager, ShouldSpawn)
{
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "SpawnTest",
      "enemies": {
        "dummy": {
          "hp": 1,
          "speed": -1,
          "size": { "w": 1, "h": 1 }
        }
      },
      "waves": [
        { "time": 2.0, "enemies": { "dummy": 1 } }
      ]
    })";

    ASSERT_TRUE(mgr.load(json));

    mgr.advance(1.0f);
    EXPECT_FALSE(mgr.shouldSpawn(2.0f));

    mgr.advance(1.5f);
    EXPECT_TRUE(mgr.shouldSpawn(2.0f));
}

TEST(LevelManager, LoadWithShootStraightValue)
{
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "SpawnTest",
      "enemies": {
        "smallEnemy": {
          "hp": 15,
          "speed": -80,
          "size": { "w": 65, "h": 66 },
          "sprite": "enemy",
          "spriteId": 2,
          "shoot": {
            "type": "straight",
            "cooldown": 2.0,
            "projectileSpeed": 100,
            "damage": 50,
            "muzzle": { "x": 0, "y": 20 }
          }
        }
      },
      "waves": [
        { "time": 2.0, "enemies": { "smallEnemy": 1 } }
      ]
    }
)";

    ASSERT_TRUE(mgr.load(json));

    const auto &level = mgr.getCurrentLevel();
    ASSERT_EQ(level.enemyTypes.size(), 1);

    const auto &enemy = level.enemyTypes.at("smallEnemy");
    ASSERT_EQ(enemy.shoot.cooldown, 2.0f);
    ASSERT_EQ(enemy.shoot.projectileSpeed, 100.f);
    ASSERT_EQ(enemy.shoot.damage, 50);
    ASSERT_EQ(enemy.shoot.muzzle.first, 0);
    ASSERT_EQ(enemy.shoot.muzzle.second, 20);
}

TEST(LevelManager, LoadWithShootDiagonalValue)
{
    Game::LevelManager mgr;

    const std::string json = R"(
    {
      "name": "SpawnTest",
      "enemies": {
        "smallEnemy": {
          "hp": 15,
          "speed": -80,
          "size": { "w": 65, "h": 66 },
          "sprite": "enemy",
          "spriteId": 2,
          "shoot": {
            "type": "diagonal",
            "cooldown": 2.0,
            "projectileSpeed": 100,
            "damage": 50,
            "muzzle": { "x": 0, "y": 20 },
            "angles": [45, -45]
          }
        }
      },
      "waves": [
        { "time": 2.0, "enemies": { "smallEnemy": 1 } }
      ]
    }
)";

    ASSERT_TRUE(mgr.load(json));

    const auto &level = mgr.getCurrentLevel();
    ASSERT_EQ(level.enemyTypes.size(), 1);

    const auto &enemy = level.enemyTypes.at("smallEnemy");
    ASSERT_EQ(enemy.shoot.cooldown, 2.0f);
    ASSERT_EQ(enemy.shoot.projectileSpeed, 100.f);
    ASSERT_EQ(enemy.shoot.damage, 50);
    ASSERT_EQ(enemy.shoot.muzzle.first, 0);
    ASSERT_EQ(enemy.shoot.muzzle.second, 20);
    ASSERT_EQ(enemy.shoot.angles.size(), 2);
    EXPECT_EQ(enemy.shoot.angles.at(0), 45);
    EXPECT_EQ(enemy.shoot.angles.at(1), -45);
}