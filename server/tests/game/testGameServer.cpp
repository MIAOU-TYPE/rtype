/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** testGameServer
*/

#include <gtest/gtest.h>
#include "game/gameServer/GameServer.hpp"
#include "MockServer.hpp"
#include "MockSessionManager.hpp"

TEST(GameServer, creates_player_on_connect)
{
    auto sessions = std::make_shared<MockSessionManager>();
    auto server = std::make_shared<MockServer>();

    Game::GameServer gs(sessions, server);

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

    Game::GameServer gs(sessions, server);

    gs.onPlayerConnect(1);
    gs.onPlayerDisconnect(1);

    Game::InputComponent input{};
    input.left = true;

    EXPECT_NO_THROW(gs.onPlayerInput(1, input));
    EXPECT_NO_THROW(gs.update(1.0f));
    SUCCEED();
}