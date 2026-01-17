/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShootSystem
*/

#pragma once
#include "Health.hpp"
#include "HomingProjectile.hpp"
#include "Id.hpp"
#include "InputComponent.hpp"
#include "WeaponConfig.hpp"
#include "World.hpp"

#include <cmath>
#include <numbers>
#include "AIShoot.hpp"

namespace Game
{
    /**
     * @class AIShootSystem
     * @brief System that handles AI shooting behavior.
     */
    class AIShootSystem {
      public:
        /**
         * @brief Update all AI shooting entities in the game world.
         * @param world The game world containing entities.
         * @param dt The delta time since the last update.
         */
        static void update(IGameWorld &world, float dt);

        /**
         * @brief Handle spread projectile shooting pattern.
         * @param world The game world containing entities.
         * @param ent The entity that is shooting.
         * @param shoot The AIShoot component of the entity.
         * @param posX The X position of the muzzle.
         * @param posY The Y position of the muzzle.
         * @param weapon The WeaponConfig component of the entity.
         */
        static void spreadProjectile(IGameWorld &world, Ecs::Entity ent, const Ecs::AIShoot &shoot, float posX,
            float posY, const Ecs::WeaponConfig &weapon);
    };
} // namespace Game