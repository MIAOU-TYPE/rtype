/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LaserPowerUpSystem
*/

#pragma once

#include "Collision.hpp"
#include "Damage.hpp"
#include "Drawable.hpp"
#include "IGameWorld.hpp"
#include "Id.hpp"
#include "LaserPowerUp.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @class LaserPowerUpSystem
     * @brief System that manages the laser power-up effect.
     *
     * Creates a laser beam entity when activated and maintains it for 5 seconds.
     * The laser beam destroys all enemies it touches.
     */
    class LaserPowerUpSystem {
      public:
        /**
         * @brief Update the laser power-up system.
         *
         * Manages laser creation, duration, and destruction.
         *
         * @param world The game world containing all entities.
         * @param dt Delta time since last update (seconds).
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
