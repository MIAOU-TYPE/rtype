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
    class AssetLoader {
      public:
        static void load(const std::shared_ptr<Graphics::ITextureManager> &textureManager,
            const std::shared_ptr<Engine::SpriteRegistry> &spriteRegistry);
    };
} // namespace Utils