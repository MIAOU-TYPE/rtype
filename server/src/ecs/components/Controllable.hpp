/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Controllable
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Controllable
     * @brief Marks an entity as controllable by a player.
     */
    struct Controllable {
        int playerId = -1; ///< ID of the controlling player
    };
} // namespace Ecs