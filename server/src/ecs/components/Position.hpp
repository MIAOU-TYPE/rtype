/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Position
*/

#pragma once
#include <cstdint>

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
        float x = 0.f;   ///> X coordinate
        float y = 0.f;   ///> Y coordinate
        uint8_t z = 1.f; ///> Z coordinate of the entity
    };
} // namespace Ecs