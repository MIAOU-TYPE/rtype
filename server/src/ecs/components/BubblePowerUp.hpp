/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BubblePowerUp
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
     * @struct BubblePowerUp
     * @brief Component for managing bubble shield power-up state on players
     *
     * This component tracks the bubble shield power-up that creates a protective
     * barrier around the player. The shield can block up to 3 enemy projectiles
     * before disappearing.
     */
    struct BubblePowerUp {
        bool isActive = false;                             ///> Whether the bubble shield is currently active
        int hitsRemaining = 0;                             ///> Number of projectile hits the shield can still block
        static constexpr int maxHits = 3;                  ///> Maximum number of hits the shield can block
        std::optional<Entity> bubbleEntity = std::nullopt; ///> The visual bubble entity attached to player
    };
} // namespace Ecs
