/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerPowerUp
*/

#pragma once

#include "Entity.hpp"

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct PlayerPowerUp
     * @brief Component tracking the power-up state for a player.
     *
     * This component manages the special weapon state for a player, including
     * whether they have a power-up equipped, the cooldown timer, and readiness state.
     */
    struct PlayerPowerUp {
        bool hasPowerUp = false;         ///> Whether the player has a power-up equipped
        float cooldown = 0.f;            ///> Current cooldown timer (seconds)
        float maxCooldown = 5.f;         ///> Maximum cooldown duration (seconds)
        bool isReady = false;            ///> Whether the special weapon is ready to fire
        bool hasBar = false;             ///> Whether the cooldown bar is currently displayed
        std::optional<Entity> barEntity; ///> The entity ID of the cooldown bar
    };
} // namespace Ecs
