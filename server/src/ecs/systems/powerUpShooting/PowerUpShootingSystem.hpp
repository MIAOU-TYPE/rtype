/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpShootingSystem
*/

#pragma once

#include "Collision.hpp"
#include "Damage.hpp"
#include "Drawable.hpp"
#include "IGameWorld.hpp"
#include "InputComponent.hpp"
#include "Lifetime.hpp"
#include "PlayerPowerUp.hpp"
#include "Position.hpp"
#include "Projectile.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @class PowerUpShootingSystem
     * @brief System responsible for power-up weapon cooldown and special shooting.
     *
     * This system handles two main responsibilities:
     * 1. Manages the cooldown/reload timer for the power-up special weapon
     * 2. Fires the special weapon when the player presses the power-up key (E)
     *
     * The special weapon fires a larger, more powerful projectile that deals
     * increased damage to enemies.
     */
    class PowerUpShootingSystem {
      public:
        /**
         * @brief Update the power-up shooting system.
         *
         * Updates cooldown timers for all players with power-ups and handles
         * special weapon firing when the input is detected.
         *
         * @param world The game world containing all entities.
         * @param dt Delta time since last update (seconds).
         */
        static void update(IGameWorld &world, float dt);

      private:
        /**
         * @brief Create a power-up projectile entity.
         *
         * Spawns a large projectile with increased damage and different sprite.
         *
         * @param world The game world to spawn the projectile in.
         * @param x Starting X position.
         * @param y Starting Y position.
         * @param vx Horizontal velocity.
         * @param vy Vertical velocity.
         * @param shooterId The entity ID of the shooter (player).
         */
        static void createPowerUpProjectile(IGameWorld &world, float x, float y, float vx, float vy, size_t shooterId);
    };
} // namespace Game
