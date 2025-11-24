/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLTextureManager
*/

#include "SFMLTextureManager.hpp"
#include "SFMLRenderer.hpp"

using namespace Graphics;

SFMLSprite::SFMLSprite(sf::Sprite sprite) : _sprite(std::move(sprite))
{
}

void SFMLSprite::setPosition(float x, float y)
{
    _sprite.setPosition(x, y);
}

float SFMLSprite::getWidth() const
{
    return _sprite.getGlobalBounds().width;
}

float SFMLSprite::getHeight() const
{
    return _sprite.getGlobalBounds().height;
}

void SFMLSprite::setScale(float scaleX, float scaleY)
{
    _sprite.setScale(scaleX, scaleY);
}

const sf::Sprite &SFMLSprite::getSFMLSprite() const
{
    return _sprite;
}

bool SFMLTextureManager::loadTexture(const std::string &filePath)
{
    if (_textures.find(filePath) != _textures.end()) {
        return true;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        return false;
    }

    _textures[filePath] = std::move(texture);
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

void SFMLTextureManager::renderSprite(const ISprite &sprite)
{
    if (!_renderer) {
        return;
    }

    const auto *sfmlSprite = dynamic_cast<const SFMLSprite *>(&sprite);
    if (sfmlSprite) {
        auto *sfmlRenderer = dynamic_cast<SFMLRenderer *>(_renderer.get());
        if (sfmlRenderer) {
            sfmlRenderer->drawSprite(sfmlSprite->getSFMLSprite());
        }
    }
}

void SFMLTextureManager::setRenderer(std::shared_ptr<IRenderer> renderer)
{
    _renderer = renderer;
}
