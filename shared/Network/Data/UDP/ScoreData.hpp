/**
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Score
*/

#pragma once
#include "HeaderData.hpp"

#pragma pack(push, 1)

/**
 * @brief Structure representing score data sent from server to clients.
 * @details This structure contains the header information and the new score of the player.
 */
struct ScoreData {
    HeaderData header; ///> The header of the score packet.
    uint32_t score;    ///> The new score of the player.
};

#pragma pack(pop)

static_assert(sizeof(ScoreData) == 8, "ScoreData layout mismatch");