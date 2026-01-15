/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** assetLoader
*/

#include "AssetLoader.hpp"

namespace Utils
{
    void AssetLoader::load(const std::shared_ptr<Graphics::ITextureManager> &textureManager,
        const std::shared_ptr<Engine::SpriteRegistry> &spriteRegistry)
    {
        Engine::SpriteLoader::loadFromFile("client/assets/json/background_space.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/boss.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/boss2.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy2.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy3.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/explose.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/obstacle.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/missile.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/missile_enemy.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/missile_enemy2.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/player.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player2.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player3.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player4.json", *spriteRegistry);

        spriteRegistry->forEach([&](const unsigned int spriteId, Engine::SpriteDefinition &def) {
            def.textureHandle = textureManager->load(def.texturePath);
            if (def.textureHandle == Graphics::InvalidTexture) {
                std::cerr << "[AssetLoader] Failed to load texture for sprite " << spriteId << ": " << def.texturePath
                          << std::endl;
            }
        });
    }
} // namespace Utils