/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem
*/

#pragma once

#include "World.hpp"

namespace Game
{
    /**
     * @brief System responsible for managing entity lifetimes.
     *
     * Decreases LifetimeComponent values and removes entities when their lifetime expires.
     */
    class LifetimeSystem {
      public:
        /**
         * @brief Update lifetimes and remove expired entities.
         *
         * @param world The game world containing components.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game
