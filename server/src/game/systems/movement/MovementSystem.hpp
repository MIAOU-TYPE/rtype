/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MovementSystem
*/

#pragma once

#include "game/world/World.hpp"

namespace Game
{
    /**
     * @brief System responsible for moving entities.
     *
     * Reads Velocity components and applies them to Position components.
     * Should be executed after InputSystem.
     */
    class MovementSystem {
      public:
        /**
         * @brief Apply velocity to positions using delta-time.
         *
         * @param world The ECS world.
         * @param dt    Time elapsed since last frame (seconds).
         */
        static void update(IGameWorld &world, float dt);
    };

} // namespace Game
