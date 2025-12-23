/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem
*/

#pragma once

#include "Collision.hpp"
#include "Damage.hpp"
#include "Damageable.hpp"
#include "InputComponent.hpp"
#include "Lifetime.hpp"
#include "Position.hpp"
#include "Velocity.hpp"
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
    };
} // namespace Game
