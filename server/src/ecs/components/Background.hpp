/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** Background
*/

#pragma once

/**
 * @namespace Ecs
 * @brief Entity Component System namespace
 */
namespace Ecs
{
    /**
     * @struct Background
     * @brief Defines the background properties of an entity.
     */
    struct Background {
        float scrollSpeed = 0.f;   ///> Speed at which the background scrolls
        float tileWidth = 0.f;     ///> Width of the background tile
        float tileHeight = 0.f;    ///> Height of the background tile
        float screenWidth = 900.f; ///> Width of the game screen
        float screenHeight = 600.f;///> Height of the game screen
        int depth = 0;             ///> Depth layer of the background
        int tileIndex = 0;         ///> Index of the tile (0 or 1)
    };
} // namespace Ecs