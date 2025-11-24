/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Collision
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Collision
     * @brief Defines the collision box of an entity.
     */
    struct Collision {
        float width = 0.f;   //< Collision width
        float height = 0.f;  //< Collision height
    };
} // namespace Ecs