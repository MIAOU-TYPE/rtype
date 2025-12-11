/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lifetime
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Lifetime
     * @brief Represents the remaining lifetime of an entity.
     */
    struct Lifetime {
        float remaining = 1.f; ///> Remaining lifetime in seconds
    };
} // namespace Ecs
