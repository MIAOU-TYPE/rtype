/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUp
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct PowerUp
     * @brief Component marking an entity as a collectible power-up.
     *
     * This component is attached to power-up entities floating in the game world.
     * When a player collides with a power-up entity, it gets collected and attached
     * to the player's ship.
     */
    struct PowerUp {
        bool collected = false; ///> Whether the power-up has been collected by a player
    };
} // namespace Ecs
