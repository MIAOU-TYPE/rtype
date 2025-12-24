/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderCommand
*/

#pragma once
#include "AnimationFrame.hpp"
#include "ITextureManager.hpp"
#include "ITextureManager.hpp"
#include "Position.hpp"

namespace Engine
{
    /**
     * @brief A rectangle defined by floating point values.
     */
    struct FloatRect {
        float x; ///> The x-coordinate of the rectangle's top-left corner.
        float y; ///> The y-coordinate of the rectangle's top-left corner.
        float w; ///> The width of the rectangle.
        float h; ///> The height of the rectangle.

        /**
         * @brief Check if a point is inside the rectangle.
         *
         * @param px The x-coordinate of the point.
         * @param py The y-coordinate of the point.
         * @return true if the point is inside the rectangle, false otherwise.
         */
        [[nodiscard]] bool contains(const float px, const float py) const noexcept
        {
            return px >= x && px <= x + w && py >= y && py <= y + h;
        }
    };

    /**
     * @brief A command to render a texture at a specific position with a given frame.
     */
    struct RenderCommand {
        Graphics::TextureHandle textureId; ///> Handle to the texture to be rendered
        IntRect frame;                     ///> Rectangle defining the frame within the texture
        Position position;                 ///> Position where the texture should be rendered
        Position scale{1.f, 1.f};          ///> Scale factors for rendering the texture

        /**
         * @brief Get the bounding rectangle of the render command.
         *
         * @return A FloatRect representing the bounds of the rendered texture.
         */
        [[nodiscard]] FloatRect bounds() const noexcept
        {
            return {position.x, position.y, static_cast<float>(frame.w), static_cast<float>(frame.h)};
        }
    };
} // namespace Engine