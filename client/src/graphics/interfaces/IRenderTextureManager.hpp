/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IRenderTextureManager
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <exception>
#include <string>

namespace Graphics
{
    /**
     * @class IRenderTextureManager
     * @brief Interface for managing render textures for post-processing effects.
     */
    class IRenderTextureManager {
      public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IRenderTextureManager() = default;

        /**
         * @brief Check if render texture is available and initialized.
         * @return True if render texture is ready to use.
         */
        [[nodiscard]] virtual bool isAvailable() const noexcept = 0;

        /**
         * @brief Initialize or resize the render texture to match window size.
         * @param width New width for the render texture.
         * @param height New height for the render texture.
         * @return True if resize was successful.
         */
        [[nodiscard]] virtual bool resize(unsigned int width, unsigned int height) = 0;

        /**
         * @brief Clear the render texture.
         */
        virtual void clear() const = 0;

        /**
         * @brief Finalize rendering to texture and prepare for display.
         */
        virtual void display() const = 0;

        /**
         * @brief Get the underlying SFML RenderTexture for drawing operations.
         * @return Pointer to the SFML RenderTexture, or nullptr if not available.
         */
        [[nodiscard]] virtual sf::RenderTexture *getRenderTexture() const noexcept = 0;

        /**
         * @brief Get the sprite representing the render texture for final display.
         * @return Sprite containing the rendered texture.
         */
        [[nodiscard]] virtual sf::Sprite getSprite() const = 0;
    };
} // namespace Graphics
