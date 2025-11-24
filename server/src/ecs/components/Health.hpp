/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Health
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Health
     * @brief Stores the health of an entity.
     */
    struct Health {
        int hp = 100;    //< Current health points
        int maxHp = 100; //< Maximum health points
    };
} // namespace Ecs