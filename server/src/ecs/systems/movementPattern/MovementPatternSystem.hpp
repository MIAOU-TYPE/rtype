/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementPatternSystem
*/

#pragma once

#include <cmath>
#include "World.hpp"

namespace Game
{
    /**
     * @brief System responsible for updating velocity based on movement patterns.
     *
     * Reads MovementPattern components and updates Velocity components accordingly.
     * Should be executed before MovementSystem.
     */
    class MovementPatternSystem {
      public:
        /**
         * @brief Update velocities based on movement patterns.
         *
         * @param world The ECS world.
         * @param dt    Time elapsed since last frame (seconds).
         */
        static void update(IGameWorld &world, float dt);
    };

} // namespace Game