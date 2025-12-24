/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CollisionSystem
*/

#pragma once
#include "AIBrain.hpp"
#include "Collision.hpp"
#include "Damage.hpp"
#include "KillScore.hpp"
#include "Projectile.hpp"
#include "World.hpp"

namespace Game
{
    /**
     * @brief System that handles collisions between entities in the game world.
     * It checks for collisions and applies damage accordingly.
     */
    class CollisionSystem {
      public:
        /**
         * @brief Update the collision system for the given game world.
         *
         * @param world The game world containing entities to check for collisions.
         */
        static void update(IGameWorld &world);
    };
} // namespace Game
