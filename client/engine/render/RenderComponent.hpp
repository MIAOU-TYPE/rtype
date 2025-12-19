/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderComponent
*/

#pragma once
#include "ITextureManager.hpp"

namespace Engine
{
    /**
     * @struct RenderComponent
     * @brief Component that holds rendering information for an entity.
     */
    struct RenderComponent {
        Graphics::TextureHandle texture; ///> Handle to the texture used for rendering
    };
} // namespace Engine