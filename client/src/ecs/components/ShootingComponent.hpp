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
        bool wantsToShoot = false;  ///> True when the entity wants to fire and cooldown has elapsed
        float cooldown = 0.f;       ///> Time remaining until the next shot can be fired
        float fireRate = 0.25f;     ///> Time between shots in seconds
    };
} // namespace Ecs
