/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** InputComponent
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct InputComponent
     * @brief Stores the current input state for a controllable entity.
     */
    struct InputComponent {
        bool up = false;       ///> True when moving up is active
        bool down = false;     ///> True when moving down is active
        bool left = false;     ///> True when moving left is active
        bool right = false;    ///> True when moving right is active
        bool shoot = false;    ///> True when shooting is active
    };
} // namespace Ecs
