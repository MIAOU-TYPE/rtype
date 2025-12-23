/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AIShoot
*/

#pragma once
#include <vector>

namespace Ecs
{
    /**
     * @struct AIShoot
     * @brief Component that defines AI shooting behavior.
     */
    struct AIShoot {
        /**
         * @enum Type
         * @brief Different types of shooting patterns.
         */
        enum class Type {
            Straight, ///> Shoots projectiles in a straight line
            Diagonal, ///> Shoots projectiles at diagonal angles
            Spread    ///> Shoots multiple projectiles in a spread pattern
        };

        Type type;                      ///> Type of shooting pattern
        float cooldown;                 ///> Time between shots
        float timer;                    ///> Internal timer to track cooldown
        float projectileSpeed;          ///> Speed of the projectiles
        int damage = 0;                 ///> Damage dealt by each projectile
        std::pair<float, float> muzzle; ///> Muzzle offset (x, y)
        std::vector<float> angles;      ///> Angles at which projectiles are fired
    };
} // namespace Ecs