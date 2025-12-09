/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** DamageSystem
*/

#pragma once
#include "World.hpp"

namespace Game
{
    /**
     * @brief System that handles damage application between entities in the game world.
     * It processes damage events and updates entity health accordingly.
     */
    class DamageSystem {
      public:
        /**
         * @brief Update the damage system for the given game world.
         *
         * @param world The game world containing entities to process for damage.
         */
        static void update(IGameWorld &world);
    };
} // namespace Game
