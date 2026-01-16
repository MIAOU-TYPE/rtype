/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PowerUpAttachmentSystem
*/

#pragma once

#include "IGameWorld.hpp"
#include "PlayerPowerUp.hpp"
#include "Position.hpp"
#include "PowerUp.hpp"

namespace Game
{
    /**
     * @class PowerUpAttachmentSystem
     * @brief System responsible for keeping attached power-ups following their player.
     *
     * This system updates the position of collected power-ups to follow
     * the player's ship position with a fixed offset.
     */
    class PowerUpAttachmentSystem {
      public:
        /**
         * @brief Update attached power-up positions.
         *
         * Iterates through all players with power-ups and updates
         * the position of their attached power-up entities.
         *
         * @param world The game world containing all entities.
         */
        static void update(IGameWorld &world);
    };
} // namespace Game
