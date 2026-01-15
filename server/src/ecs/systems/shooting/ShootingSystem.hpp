/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#pragma once

#include "ShootCooldown.hpp"
#include "WeaponConfig.hpp"
#include "World.hpp"

namespace Game
{
    /** @class ShootingSystem
     * @brief System that handles shooting mechanics for entities with InputComponent.
     */
    class ShootingSystem {
      public:
        /** @brief Update the shooting system.
         * @param world The game world containing all entities and components.
         */
        static void update(IGameWorld &world);

        /** @brief Update shoot cooldown timers.
         * @param world The game world containing all entities and components.
         * @param dt Delta time in seconds.
         */
        static void updateCooldowns(IGameWorld &world, float dt);
    };
} // namespace Game
