/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** HomingSystem
*/

#pragma once

#include "HomingProjectile.hpp"
#include "Position.hpp"
#include "Velocity.hpp"
#include "World.hpp"

namespace Game
{
    /**
     * @class HomingSystem
     * @brief System that handles homing projectile behavior.
     */
    class HomingSystem {
      public:
        /**
         * @brief Update all homing projectiles in the game world.
         * @param world The game world containing entities.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
