/**
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameEndData
*/

#pragma once

#include <cstdint>
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * GameEndData UDP structure
 * Sent by the server to all clients when the game ends
 * Contains the final scores of all players
 */
struct GameEndHeader {
    HeaderData header; ///> Common header data
    uint8_t count;     ///> Number of player score entries (network order)
};

/**
 * GameEndEntry UDP structure
 * Represents a single player's score entry
 */
struct GameEndEntry {
    uint32_t playerId; ///> Player ID (network order)
    uint32_t score;    ///> Player score (network order)
};

#pragma pack(pop)

static_assert(sizeof(GameEndHeader) == sizeof(HeaderData) + 1, "GameEndHeader layout mismatch");
static_assert(sizeof(GameEndEntry) == 8, "GameEndEntry layout mismatch");
