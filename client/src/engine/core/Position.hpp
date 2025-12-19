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
     * @struct Vec2
     * @brief Represents a 2D vector or position with x and y coordinates.
     */
    template <typename T>
    struct Vec2 {
        T x; ///> X coordinate
        T y; ///> Y coordinate
    };
} // namespace Engine