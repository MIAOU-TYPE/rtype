/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Direction
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Direction
     * @brief Defines a direction vector.
     */
    struct Direction {
        int dx = 0; ///> Direction on X axis
        int dy = 0; ///> Direction on Y axis
    };
} // namespace Ecs