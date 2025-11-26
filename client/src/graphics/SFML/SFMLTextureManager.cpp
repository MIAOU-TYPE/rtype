/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLTextureManager
*/

#include "SFMLTextureManager.hpp"

using namespace Graphics;

bool SFMLTextureManager::loadTexture(const std::string &filePath)
{
    if (_textures.find(filePath) != _textures.end()) {
        return true;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        return false;
    }

    _textures[filePath] = texture;
    return true;
}

std::unique_ptr<ISprite> SFMLTextureManager::createSprite(const std::string &texturePath)
{
    if (!loadTexture(texturePath)) {
        return nullptr;
    }

    sf::Sprite sprite;
    sprite.setTexture(_textures[texturePath]);

    return std::make_unique<SFMLSprite>(std::move(sprite));
}
