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
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool shoot = false;
    };
} // namespace Ecs
