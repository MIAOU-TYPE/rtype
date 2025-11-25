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

      private:
        std::unordered_map<std::string, sf::Texture> _textures = {}; ///> Map of loaded textures
    };

} // namespace Graphics
