/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Position
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Position
     * @brief Stores the 2D position of an entity.
     */
    struct Position {
        float x = 0.f; ///> X coordinate
        float y = 0.f; ///> Y coordinate
    };
} // namespace Ecs