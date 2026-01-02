/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** assetLoader
*/

#include "AssetLoader.hpp"
#include "SpriteLoader.hpp"

namespace Utils
{
    void AssetLoader::load(const std::shared_ptr<Graphics::ITextureManager> &textureManager,
        const std::shared_ptr<Engine::SpriteRegistry> &spriteRegistry)
    {
        Engine::SpriteLoader::loadFromFile("client/assets/json/background_space.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/boss.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy2.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/enemy3.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/explose.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/missile.json", *spriteRegistry);

        Engine::SpriteLoader::loadFromFile("client/assets/json/player.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player2.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player3.json", *spriteRegistry);
        Engine::SpriteLoader::loadFromFile("client/assets/json/player4.json", *spriteRegistry);

        spriteRegistry->forEach([&](const unsigned int, Engine::SpriteDefinition &def) {
            def.textureHandle = textureManager->load(def.texturePath);
        });
    }
} // namespace Utils