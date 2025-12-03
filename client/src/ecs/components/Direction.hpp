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
        float dx = 0.f; ///> Direction on X axis
        float dy = 0.f; ///> Direction on Y axis
    };
} // namespace Ecs