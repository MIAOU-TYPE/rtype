/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShootSystem
*/

#pragma once
#include "World.hpp"

#include <cmath>
#include <numbers>
#include "AIBrain.hpp"
#include "AIShoot.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "Lifetime.hpp"
#include "Projectile.hpp"

/**
 * @struct PendingProjectile
 * @brief Structure to hold data for projectiles that are pending creation.
 * Used to batch projectile creation after processing all AI shooting logic.
 */
struct PendingProjectile {
    /**
     * @brief Constructor for PendingProjectile.
     * @param x position x
     * @param y position y
     * @param vx velocity x
     * @param vy velocity y
     * @param dmg damage value
     * @param shooter shooter entity ID
     * @param colX collision width
     * @param colY collision height
     */
    PendingProjectile(const float x, const float y, const float vx, const float vy, const int dmg, const size_t shooter,
        const float colX, const float colY)
        : pos{x, y}, velocity{vx, vy}, damage{dmg}, projectile{shooter}, collision{colX, colY}
    {
    }

    Ecs::Position pos;          ///> Position of the projectile
    Ecs::Velocity velocity;     ///> Velocity of the projectile
    Ecs::Damage damage;         ///> Damage component
    Ecs::Projectile projectile; ///> Projectile component
    Ecs::Collision collision;   ///> Collision component
};

namespace Game
{
    /**
     * @class AIShootSystem
     * @brief System that handles AI shooting behavior.
     */
    class AIShootSystem {
      public:
        /**
         * @brief Update all AI shooting entities in the game world.
         * @param world The game world containing entities.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game