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
#include "SFMLSpriteManagement.hpp"
#include <unordered_map>

/**
 * @namespace Graphics
 * @brief Contains all graphics-related classes and interfaces.
 */
namespace Graphics
{
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

        /**
         * @brief Loads a font from a file.
         * @param fontPath The path to the font file.
         * @return True if the font was loaded successfully, false otherwise.
         */
        bool loadFont(const std::string &fontPath) override;

        /**
         * @brief Creates a text object using a loaded font.
         * @param fontPath The path to the font to use.
         * @param text The initial text string.
         * @param characterSize The character size in pixels.
         * @return A unique pointer to the created text object.
         */
        std::unique_ptr<IText> createText(
            const std::string &fontPath, const std::string &text = "", unsigned int characterSize = 30) override;

      private:
        std::unordered_map<std::string, sf::Texture> _textures = {};            ///> Map of loaded textures
        std::unordered_map<std::string, std::shared_ptr<sf::Font>> _fonts = {}; ///> Map of loaded fonts
    };

} // namespace Graphics
