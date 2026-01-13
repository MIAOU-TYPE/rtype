/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BackgroundSystem
*/

#pragma once

#include "Background.hpp"
#include "Position.hpp"
#include "World.hpp"

namespace Game
{
    /**
     * @brief System that manages background layers in the game.
     */
    class BackgroundSystem {
      public:
        /**
         * @brief Update the background system for the given game world.
         *
         * @param world The game world containing background entities.
         * @param deltaTime Time elapsed since the last update.
         */
        static void update(IGameWorld &world, float deltaTime);

        /**
         * @brief Reset the scroll position of background layers when they go off-screen.
         * Uses the tileWidth from each background component (loaded from level JSON).
         *
         * @param world The game world containing background entities.
         */
        static void resetScroll(IGameWorld &world);
    };
} // namespace Game