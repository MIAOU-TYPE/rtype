/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** BossSystem
*/

#pragma once

#include "AIShoot.hpp"
#include "BossPhase.hpp"
#include "Health.hpp"
#include "IGameWorld.hpp"
#include "Id.hpp"
#include "Velocity.hpp"
#include "Position.hpp"

namespace Game
{
    /**
     * @brief System responsible for managing boss behavior and phase transitions.
     */
    class BossSystem {
      public:
        /**
         * @brief Update the boss system for the given game world.
         *
         * @param world The game world containing boss entities.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);
    };
} // namespace Game