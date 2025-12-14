/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AttackSystem
*/

#pragma once

#include "IGameWorld.hpp"

namespace Game
{
    /**
     * @brief System responsible for handling attack logic in the game world.
     */
    class AttackSystem {
      public:
        /**
         * @brief Update the attack system for the given game world.
         *
         * @param world The game world to update.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
