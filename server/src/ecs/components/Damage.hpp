/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Damage
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Damage
     * @brief Represents damage dealt by an entity.
     */
    struct Damage {
        int amount = 0; ///> Damage amount
    };
} // namespace Ecs
