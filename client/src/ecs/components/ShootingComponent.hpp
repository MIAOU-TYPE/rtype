/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ShootingComponent
*/

#pragma once

namespace Ecs
{
    /**
     * @struct ShootingComponent
     * @brief Marks an entity as wanting to shoot.
     */
    struct ShootingComponent {
        bool wantsToShoot = false;
        float cooldown = 0.f;
        float fireRate = 0.25f;
    };
} // namespace Ecs
