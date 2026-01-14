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
     * When active, the player fires a continuous laser beam that destroys
     * all enemies in its path for a limited duration.
     */
    struct LaserPowerUp {
        bool isActive = false;             ///> Whether the laser is currently active
        float duration = 0.f;              ///> Current duration timer (seconds)
        float maxDuration = 5.f;           ///> Maximum duration of the laser (seconds)
        std::optional<Entity> laserEntity; ///> The entity ID of the laser beam
    };
} // namespace Ecs
