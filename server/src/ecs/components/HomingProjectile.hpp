/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** HomingProjectile
*/

#pragma once

#include <cstddef>

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct HomingProjectile
     * @brief Component that defines homing projectile behavior.
     */
    struct HomingProjectile {
        size_t targetId;      ///> ID of the target entity
        float homingStrength; ///> Strength of the homing effect
        float maxSpeed;       ///> Maximum speed of the projectile
    };
} // namespace Ecs