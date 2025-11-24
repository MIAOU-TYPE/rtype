/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Damageable
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Damageable
     * @brief Defines if an entity can receive damage.
     */
    struct Damageable {
        bool canBeDamaged = true; //< If false, entity ignores damage
    };
} // namespace Ecs