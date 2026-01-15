/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootCooldown
*/

#pragma once

namespace Ecs
{
    /**
     * @struct ShootCooldown
     * @brief Component for managing player shooting cooldown.
     */
    struct ShootCooldown {
        float cooldown = 0.25f; ///> Time between shots (in seconds)
        float timer = 0.0f;     ///> Current timer value
    };
} // namespace Ecs
