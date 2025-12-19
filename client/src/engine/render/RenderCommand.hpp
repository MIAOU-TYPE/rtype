/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderCommand
*/

#pragma once
#include <string>
#include "../core/Animation/AnimationFrame.hpp"
#include "Position.hpp"

namespace Engine
{
    /**
     * @struct RenderCommand
     * @brief Represents a command to render a texture at a specific position with a given frame
     * from a texture atlas.
     */
    struct RenderCommand {
        Graphics::TextureHandle textureId; //> Handle to the texture to be rendered
        IntRect frame;                     //> Rectangle defining the frame within the texture
        Vec2<float> position;              ///> Position where the texture should be rendered
    };

} // namespace Engine