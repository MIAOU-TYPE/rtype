/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** assetLoader
*/

#pragma once
#include "ITextureManager.hpp"
#include "SpriteRegistry.hpp"

namespace Utils
{
    /**
     * @class AssetLoader
     * @brief Utility class for loading game assets such as textures and sprites.
     */
    class AssetLoader {
      public:
        /**
         * @brief Load game assets into the provided texture manager and sprite registry.
         * @param textureManager Shared pointer to the texture manager for loading textures.
         * @param spriteRegistry Shared pointer to the sprite registry for registering sprites.
         */
        static void load(const std::shared_ptr<Graphics::ITextureManager> &textureManager,
            const std::shared_ptr<Engine::SpriteRegistry> &spriteRegistry);
    };
} // namespace Utils