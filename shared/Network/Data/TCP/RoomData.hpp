/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomData
*/

#pragma once
#include <cstdint>
#include <string>
#include "GameConfig.hpp"

/**
 * @struct RoomData
 * @brief Structure representing information about a game room.
 */
struct RoomData {
    uint32_t roomId;                      ///> Unique identifier for the room
    std::string roomName;                 ///> Name of the room
    size_t currentPlayers;                ///> Current number of players in the room
    size_t maxPlayers;                    ///> Maximum number of players allowed in the room
    Engine::GameConfig gameConfig;        ///> Game configuration for the room
    std::vector<std::string> playerNames; ///> List of player names in the room
};
