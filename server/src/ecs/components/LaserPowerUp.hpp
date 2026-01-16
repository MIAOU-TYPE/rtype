/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LaserPowerUp
*/

#pragma once

#include <optional>
#include "Entity.hpp"

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct LaserPowerUp
     * @brief Component tracking the laser power-up state for a player.
     *
     * When this component is present, the player fires a continuous laser beam that destroys
     * all enemies in its path for a limited duration.
     * The component is removed when the laser duration expires.
     */
    struct LaserPowerUp {
        float duration = 0.f;    ///> Current duration timer (seconds)
        float maxDuration = 5.f; ///> Maximum duration of the laser (seconds)
    };
} // namespace Ecs
