/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ScoreEntry
*/

#pragma once
#include <string>

/**
 * @struct ScoreEntry
 * @brief Structure representing a user's score entry.
 */
struct ScoreEntry {
    std::string username; ///> The username of the player
    int score;            ///> The score achieved by the player
};
