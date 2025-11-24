/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Velocity
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Velocity
     * @brief Defines the movement speed of an entity.
     */
    struct Velocity {
        float vx = 0.f; ///> Speed on X axis
        float vy = 0.f; ///> Speed on Y axis
    };
} // namespace Ecs