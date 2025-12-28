/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** KillScore
*/

#pragma once

namespace Ecs
{
    /**
     * @struct KillScore
     * @brief Component that holds the score awarded for killing an entity.
     */
    struct KillScore {
        unsigned int score = 0; ///> Score awarded for killing the entity
    };
} // namespace Ecs