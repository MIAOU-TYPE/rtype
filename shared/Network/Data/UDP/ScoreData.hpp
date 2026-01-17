/**
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ScoreData
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing score data sent from server to clients.
 * @details Contains the net player id and the player score.
 */
struct ScoreData {
    HeaderData header;
    uint32_t playerId; ///> net player id (same as ACCEPT)
    uint32_t score;    ///> score value
};

#pragma pack(pop)

static_assert(sizeof(ScoreData) == sizeof(HeaderData) + 8, "ScoreData layout mismatch");