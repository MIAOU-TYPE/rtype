/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BubblePowerUpSystem
*/

#pragma once

#include "BubblePowerUp.hpp"
#include "Drawable.hpp"
#include "IGameWorld.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @class BubblePowerUpSystem
     * @brief System responsible for managing bubble shield power-up behavior
     *
     * This system handles:
     * - Creating and positioning the visual bubble sprite around the player
     * - Updating bubble position to follow the player
     * - Destroying the bubble when hits are depleted
     */
    class BubblePowerUpSystem {
      public:
        /**
         * @brief Update all active bubble shields in the game world
         *
         * Updates bubble entity positions to follow their respective players
         * and handles destruction when shield is depleted.
         *
         * @param world The game world containing all entities
         */
        static void update(IGameWorld &world);
    };
} // namespace Game
