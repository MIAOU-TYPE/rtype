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
        int damage = 0;       //< Damage value
        float range = 0.f;    //< Attack range
        float cooldown = 0.f; //< Cooldown between attacks
    };
} // namespace Ecs