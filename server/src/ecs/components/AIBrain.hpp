/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AIBrain
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct AIBrain
     * @brief Stores AI state and internal timer.
     */
    struct AIBrain {
        int state = 0;     //< Current AI state
        float timer = 0.f; //< Timer used for decision updates
    };
} // namespace Ecs