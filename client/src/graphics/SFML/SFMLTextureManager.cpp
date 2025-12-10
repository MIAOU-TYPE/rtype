/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLTextureManager
*/

#include "SFMLTextureManager.hpp"
#include "SFMLText.hpp"

using namespace Graphics;

bool SFMLTextureManager::loadTexture(const std::string &filePath)
{
    if (_textures.contains(filePath)) {
        return true;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        return false;
    }

    _textures.emplace(filePath, std::move(texture));
    return true;
}

std::unique_ptr<ISprite> SFMLTextureManager::createSprite(const std::string &texturePath)
{
    if (!loadTexture(texturePath)) {
        return nullptr;
    }

    sf::Sprite sprite(_textures.at(texturePath));

    return std::make_unique<SFMLSprite>(std::move(sprite));
}

bool SFMLTextureManager::loadFont(const std::string &fontPath)
{
    if (_fonts.contains(fontPath)) {
        return true;
    }

    auto font = std::make_shared<sf::Font>();
    if (!font->openFromFile(fontPath)) {
        return false;
    }

    _fonts.emplace(fontPath, font);
    return true;
}

std::unique_ptr<IText> SFMLTextureManager::createText(
    const std::string &fontPath, const std::string &text, unsigned int characterSize)
{
    if (!loadFont(fontPath)) {
        return nullptr;
    }

    return std::make_unique<SFMLText>(_fonts.at(fontPath), text, characterSize);
}
