/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testLevelManager
*/

#include <gtest/gtest.h>
#include "LevelManager.hpp"

TEST(LevelManager, LoadFromMemory_ValidJSON)
{
    const std::string jsonLevel = R"(
    {
      "name": "TestLevel",
      "duration": 30,
      "ennemies": {
        "small": { "hp": 10, "speed": -50, "size": { "w": 10, "h": 10 }, "sprite": "s.png" }
      },
      "waves": [
        { "time": 1.5, "enemies": { "small": 3 } }
      ]
    })";

    Game::LevelManager mgr;
    ASSERT_TRUE(mgr.loadFromMemory(jsonLevel));

    const Game::Level &lvl = mgr.getCurrentLevel();

    EXPECT_EQ(lvl.name, "TestLevel");
    EXPECT_FLOAT_EQ(lvl.duration, 30);

    ASSERT_EQ(lvl.enemyTypes.size(), 1);
    ASSERT_TRUE(lvl.enemyTypes.contains("small"));

    const auto &e = lvl.enemyTypes.at("small");

    EXPECT_EQ(e.hp, 10);
    EXPECT_FLOAT_EQ(e.speed, -50.f);
    EXPECT_FLOAT_EQ(e.colW, 10.f);
    EXPECT_FLOAT_EQ(e.colH, 10.f);
    EXPECT_EQ(e.sprite, "s.png");

    ASSERT_EQ(lvl.waves.size(), 1);
    EXPECT_FLOAT_EQ(lvl.waves[0].time, 1.5f);

    ASSERT_EQ(lvl.waves[0].groups.size(), 1);
    EXPECT_EQ(lvl.waves[0].groups[0].type, "small");
    EXPECT_EQ(lvl.waves[0].groups[0].count, 3);
}

TEST(LevelManager, LoadFromMemory_InvalidJSON)
{
    Game::LevelManager mgr;

    const std::string broken = R"({ "name": "Bad", BAD )";

    EXPECT_FALSE(mgr.loadFromMemory(broken));
}

TEST(LevelManager, TimeAdvancement)
{
    Game::LevelManager mgr;
    mgr.loadFromMemory("{\"name\":\"X\",\"duration\":10}");

    EXPECT_FLOAT_EQ(mgr.getTime(), 0.f);

    mgr.advance(0.5f);
    EXPECT_FLOAT_EQ(mgr.getTime(), 0.5f);

    mgr.reset();
    EXPECT_FLOAT_EQ(mgr.getTime(), 0.f);
}

TEST(LevelManager, ShouldSpawn)
{
    Game::LevelManager mgr;
    mgr.loadFromMemory("{\"name\":\"X\",\"duration\":10}");

    mgr.advance(1.0f);
    EXPECT_TRUE(mgr.shouldSpawn(0.5f));
    EXPECT_FALSE(mgr.shouldSpawn(2.0f));

    mgr.advance(2.0f);
    EXPECT_TRUE(mgr.shouldSpawn(2.5f));
}
