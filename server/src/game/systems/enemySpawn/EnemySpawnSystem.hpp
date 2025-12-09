/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemySpawnSystem
*/

#pragma once
#include <random>
#include "Collision.hpp"
#include "Damageable.hpp"
#include "World.hpp"

namespace Game
{
    /** @class EnemySpawnSystem
     * @brief System responsible for spawning enemies at regular intervals.
     */
    class EnemySpawnSystem {
      public:
        /** @brief Update the enemy spawn system.
         * @param world The game world.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);

      private:
        static float _timer; ///> Timer to track spawn intervals.
    };
} // namespace Game
