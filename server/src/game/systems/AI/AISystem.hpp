/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AISystem
*/

#pragma once

#include "AIBrain.hpp"
#include "World.hpp"

namespace Game
{
    /**
     * @brief System that updates AI-controlled entities in the game world.
     */
    class AISystem {
      public:
        /**
         * @brief Update all AI entities in the game world.
         *
         * @param world The game world containing entities.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
