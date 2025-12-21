/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Drawable
*/

#pragma once
#include <string>

namespace Engine
{
    /**
     * @struct Drawable
     * @brief Component that holds the sprite identifier for rendering an entity.
     */
    struct Drawable {
        int spriteId; ///> Identifier for the sprite to be used for rendering
    };
} // namespace Engine
