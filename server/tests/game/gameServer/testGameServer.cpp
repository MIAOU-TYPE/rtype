/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testGameServer
*/

#include <gtest/gtest.h>

#define private public
#include "GameServer.hpp"
#undef private

#include "MockServer.hpp"
#include "MockSessionManager.hpp"
#include "UDPPacket.hpp"

TEST(GameServer, creates_player_on_connect)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto server = std::make_shared<MockServer>();
    auto factory = std::make_shared<Net::Factory::PacketFactory>(std::make_shared<Net::UDPPacket>());

    Game::GameServer gs(sessions, server, factory);

    ASSERT_TRUE(gs._levelManager.load(R"(
    {
      "name": "TestLevel",
      "duration": 10,
      "enemies": {
        "small": {
          "hp": 10,
          "speed": -50,
          "size": { "w": 10, "h": 10 }
        }
      },
      "waves": [
        { "time": 1.0, "enemies": { "small": 2 } }
      ]
    }
    )"));

    gs._levelManager.reset();

    gs.onPlayerConnect(42);

    Game::InputComponent input{};
    input.right = true;
    gs.onPlayerInput(42, input);

    gs.update(1.0f);
    SUCCEED();
}

TEST(GameServer, destroys_player_on_disconnect)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto server = std::make_shared<MockServer>();
    auto factory = std::make_shared<Net::Factory::PacketFactory>(std::make_shared<Net::UDPPacket>());

    Game::GameServer gs(sessions, server, factory);

    ASSERT_TRUE(gs._levelManager.load(R"(
    {
      "name": "TestLevel",
      "duration": 10,
      "enemies": {
        "small": {
          "hp": 10,
          "speed": -50,
          "size": { "w": 10, "h": 10 }
        }
      },
      "waves": [
        { "time": 1.0, "enemies": { "small": 1 } }
      ]
    }
    )"));

    gs._levelManager.reset();

    gs.onPlayerConnect(1);
    gs.onPlayerDisconnect(1);

    Game::InputComponent input{};
    input.left = true;

    EXPECT_NO_THROW(gs.onPlayerInput(1, input));
    EXPECT_NO_THROW(gs.update(1.0f));
}
