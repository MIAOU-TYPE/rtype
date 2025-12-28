/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Render
*/

#pragma once
#include "ITextureManager.hpp"

namespace Ecs
{
    /**
     * @struct Render
     * @brief Component that holds the texture handle for rendering an entity.
     */
    struct Render {
        Graphics::TextureHandle texture = Graphics::InvalidTexture; ///> Handle to the texture used for rendering
    };
} // namespace Ecs
