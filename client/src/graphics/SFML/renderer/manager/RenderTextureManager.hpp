/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RenderTextureManager
*/

#pragma once

#include <iostream>
#include <memory>

#include "IRenderTextureManager.hpp"

namespace Graphics
{
    /**
     * @class RenderTextureManager
     * @brief SFML implementation of IRenderTextureManager for managing render texture post-processing.
     */
    class RenderTextureManager final : public IRenderTextureManager {
      public:
        /**
         * @brief Constructor for RenderTextureManager.
         * @param initialWidth Initial width of the render texture.
         * @param initialHeight Initial height of the render texture.
         */
        RenderTextureManager(unsigned int initialWidth, unsigned int initialHeight);

        /**
         * @brief Check if render texture is available and initialized.
         * @return True if render texture is ready to use.
         */
        [[nodiscard]] bool isAvailable() const noexcept override;

        /**
         * @brief Initialize or resize the render texture to match window size.
         * @param width New width for the render texture.
         * @param height New height for the render texture.
         * @return True if resize was successful.
         */
        [[nodiscard]] bool resize(unsigned int width, unsigned int height) override;

        /**
         * @brief Clear the render texture.
         */
        void clear() const override;

        /**
         * @brief Finalize rendering to texture and prepare for display.
         */
        void display() const override;

        /**
         * @brief Get the underlying SFML RenderTexture for drawing operations.
         * @return Pointer to the SFML RenderTexture, or nullptr if not available.
         */
        [[nodiscard]] sf::RenderTexture *getRenderTexture() noexcept override;

        /**
         * @brief Get the sprite representing the render texture for final display.
         * @return Sprite containing the rendered texture.
         */
        [[nodiscard]] sf::Sprite getSprite() const override;

      private:
        std::unique_ptr<sf::RenderTexture> _renderTexture = nullptr; ///> SFML render texture for post-processing
    };
} // namespace Graphics
