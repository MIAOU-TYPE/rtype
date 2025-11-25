/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLTextureManager
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "ITextureManager.hpp"
#include <unordered_map>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
    /**
     * @class SFMLSprite
     * @brief SFML implementation of the ISprite interface.
     */
    class SFMLSprite : public ISprite {
      public:
        /**
         * @brief Constructor for SFMLSprite.
         * @param sprite The SFML sprite to wrap.
         */
        explicit SFMLSprite(sf::Sprite sprite);

        /**
         * @brief Sets the position of the sprite.
         * @param x The X coordinate.
         * @param y The Y coordinate.
         */
        void setPosition(float x, float y) override;

        /**
         * @brief Gets the width of the sprite.
         * @return The width of the sprite.
         */
        float getWidth() const override;

        /**
         * @brief Gets the height of the sprite.
         * @return The height of the sprite.
         */
        float getHeight() const override;

        /**
         * @brief Sets the scale of the sprite.
         * @param scaleX The X scale factor.
         * @param scaleY The Y scale factor.
         */
        void setScale(float scaleX, float scaleY) override;

        /**
         * @brief Gets the underlying SFML sprite.
         * @return Reference to the SFML sprite.
         */
        const sf::Sprite &getSFMLSprite() const;

      private:
        sf::Sprite _sprite; ///> The underlying SFML sprite
    };

    /**
     * @class SFMLTextureManager
     * @brief SFML implementation of the ITextureManager interface.
     *
     * This class provides texture management functionality using the SFML library.
     */
    class SFMLTextureManager : public ITextureManager {
      public:
        /**
         * @brief Loads a texture from file.
         * @param filePath The path to the texture file.
         * @return True if the texture was loaded successfully, false otherwise.
         */
        bool loadTexture(const std::string &filePath) override;

        /**
         * @brief Creates a sprite from a loaded texture.
         * @param texturePath The path to the texture file.
         * @return A unique pointer to the created sprite.
         */
        std::unique_ptr<ISprite> createSprite(const std::string &texturePath) override;

      private:
        std::unordered_map<std::string, sf::Texture> _textures; ///> Map of loaded textures
    };

} // namespace Graphics
