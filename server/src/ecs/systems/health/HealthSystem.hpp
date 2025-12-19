/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** HealthSystem
*/

#pragma once

#include "Health.hpp"
#include "World.hpp"

namespace Game
{
    /** @class HealthSystem
     * @brief System responsible for managing entity health.
     */
    class HealthSystem {
      public:
        /** @brief Update the health system.
         * @param world The game world.
         */
        static void update(IGameWorld &world);
    };
} // namespace Game
