/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomData
*/

#pragma once

#pragma pack(push, 1)
#include <cstdint>
#include <string>

/**
 * @struct RoomData
 * @brief Structure representing information about a game room.
 */
struct RoomData {
    uint32_t roomId;        ///> Unique identifier for the room
    std::string roomName;      ///> Name of the room
    size_t currentPlayers; ///> Current number of players in the room
    size_t maxPlayers;     ///> Maximum number of players allowed in the room
};

#pragma pack(pop)