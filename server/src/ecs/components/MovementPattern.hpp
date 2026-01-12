/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementPattern
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct MovementPattern
     * @brief Defines the movement pattern of an entity.
     */
    struct MovementPattern {
        /**
         * @enum Type
         * @brief Types of movement patterns.
         */
        enum class Type {
            Straight, ///> Move in a straight line
            ZigZag    ///> Move in a zig-zag pattern (oscillate vertically)
        };

        Type type = Type::Straight; ///> Type of movement pattern
        float baseVx = 0.f;         ///> Base horizontal velocity
        float amplitude = 100.f;    ///> Amplitude of vertical oscillation (for zig-zag)
        float frequency = 2.f;      ///> Frequency of oscillation (for zig-zag)
        float timer = 0.f;          ///> Internal timer for pattern calculation
    };
} // namespace Ecs