/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Attack
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Attack
     * @brief Defines attack attributes of an entity.
     */
    struct Attack {
        int damage = 10;               ///> Damage value
        float cooldown = 1.f;          ///> Cooldown between attacks
        float projectileSpeed = 300.f; ///> Speed of the projectile
    };
} // namespace Ecs