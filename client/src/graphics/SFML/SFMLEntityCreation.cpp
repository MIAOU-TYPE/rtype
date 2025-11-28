/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEntityCreation
*/

#include "SFMLEntityCreation.hpp"
#include <utility>
#include "SFMLEntityDrawing.hpp"

using namespace Graphics;

GraphicalEntity::GraphicalEntity(
    float x, float y, const std::string &spriteName, std::shared_ptr<ITextureManager> textureManager)
    : _x(x), _y(y), _spriteName(spriteName), _textureManager(std::move(textureManager))
{
    if (!_textureManager) {
        throw std::runtime_error("Texture manager cannot be null");
    }

    SFMLEntityDrawing::SpriteInfo spriteInfo = SFMLEntityDrawing::getSpriteInfoFromName(spriteName);

    if (!_textureManager->loadTexture(spriteInfo.path)) {
        throw std::runtime_error("Failed to load texture: " + spriteInfo.path);
    }

    _sprite = _textureManager->createSprite(spriteInfo.path);
    if (!_sprite) {
        throw std::runtime_error("Failed to create sprite for: " + spriteName);
    }

    if (spriteInfo.width > 0.0f && spriteInfo.height > 0.0f) {
        _sprite->setTextureRect(0.0f, 0.0f, spriteInfo.width, spriteInfo.height);
    }

    _sprite->setPosition(_x, _y);
}

void GraphicalEntity::setPosition(float x, float y)
{
    _x = x;
    _y = y;
    if (_sprite) {
        _sprite->setPosition(_x, _y);
    }
}

float GraphicalEntity::getWidth() const
{
    SFMLEntityDrawing::SpriteInfo info = SFMLEntityDrawing::getSpriteInfoFromName(_spriteName);
    return info.width;
}

float GraphicalEntity::getHeight() const
{
    SFMLEntityDrawing::SpriteInfo info = SFMLEntityDrawing::getSpriteInfoFromName(_spriteName);
    return info.height;
}

void GraphicalEntity::render(const std::shared_ptr<IRenderer> &renderer)
{
    if (renderer && _sprite) {
        renderer->renderSprite(*_sprite);
    }
}