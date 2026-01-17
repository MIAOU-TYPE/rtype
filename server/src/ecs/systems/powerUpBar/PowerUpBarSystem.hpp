/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpBarSystem
*/

#pragma once

#include <algorithm>
#include "Drawable.hpp"
#include "Entity.hpp"
#include "IGameWorld.hpp"
#include "Id.hpp"
#include "PlayerPowerUp.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

namespace Game
{
    /**
     * @class PowerUpBarSystem
     * @brief System that creates and manages the power-up cooldown bar display
     *
     * Creates a visual bar entity when a player collects a power-up.
     * The bar uses sprite ID 16 (bar.png) with 17 frames animation that matches the 5s cooldown.
     */
    class PowerUpBarSystem {
      public:
        /**
         * @brief Update the power-up bar display
         * @param world Game world reference
         * @param dt Delta time
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
