/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Position
*/

#pragma once

namespace Engine
{
    /**
     * @struct Position
     * @brief Component that holds the position of an entity in 2D space.
     */
    struct Position {
        float x = 0.f;  ///> X coordinate of the entity
        float y = 0.f;  ///> Y coordinate of the entity
    };
} // namespace Engine
