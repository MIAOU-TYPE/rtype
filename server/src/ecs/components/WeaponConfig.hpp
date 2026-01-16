/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** WeaponConfig
*/

#pragma once

namespace Ecs
{
    /**
     * @struct WeaponConfig
     * @brief Component that defines weapon/projectile configuration for an entity.
     */
    struct WeaponConfig {
        unsigned int projectileSpriteId = 6; ///> Sprite ID for the projectiles this entity fires
        float cooldown = 0.25f;              ///> Time in seconds between shots
        float timer = 0.f;                   ///> Timer to track cooldown
    };
} // namespace Ecs
