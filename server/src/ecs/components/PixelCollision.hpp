/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** PixelCollision
*/

#pragma once

namespace Ecs
{
    /**
     * @struct PixelCollision
     * @brief Component for pixel-perfect collision detection.
     */
    struct PixelCollision {
        bool useAlpha = false;  ///> Whether to use alpha channel for collision detection
        float tileWidth = 0.f;  ///> Width of the collision tile
        float tileHeight = 0.f; ///> Height of the collision tile
    };
} // namespace Ecs