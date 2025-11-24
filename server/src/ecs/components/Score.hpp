/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Score
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Score
     * @brief Stores the score of an entity.
     */
    struct Score {
        int score = 0; //< Current score
        int highScore = 0; //< Highest score achieved
    };
} // namespace Ecs