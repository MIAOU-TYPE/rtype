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
#include "AIShoot.hpp"

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