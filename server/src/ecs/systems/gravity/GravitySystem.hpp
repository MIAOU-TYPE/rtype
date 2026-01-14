/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GravitySystem
*/

#pragma once

#include "IGameWorld.hpp"
#include "Events.hpp"
#include "GravityAffected.hpp"
#include "GravityField.hpp"
#include "Health.hpp"
#include "Id.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

#include <cmath>

namespace Game
{
    /**
     * @class GravitySystem
     * @brief System responsible for applying gravity field effects to entities.
     *
     * This system handles:
     * - Pulling entities toward gravity sources
     * - Applying damage when entities are too close
     * - Respecting entity immunity via GravityAffected component
     */
    class GravitySystem {
      public:
        /**
         * @brief Update the gravity system.
         * @param world The game world.
         * @param dt Delta time in seconds.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
