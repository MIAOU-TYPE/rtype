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

GraphicalEntity::GraphicalEntity(float x, float y, const std::string &spriteName,
    std::shared_ptr<ITextureManager> textureManager, const SFMLEntityDrawing &entityDrawing)
    : _x(x), _y(y), _spriteName(spriteName), _textureManager(std::move(textureManager)), _entityDrawing(entityDrawing)
{
    if (!_textureManager) {
        throw GraphicalEntityError("Texture manager cannot be null");
    }

    SpriteInfo spriteInfo = entityDrawing.getSpriteInfoFromName(spriteName);

    if (!_textureManager->loadTexture(spriteInfo.path)) {
        throw GraphicalEntityError("Failed to load texture: " + spriteInfo.path);
    }

    _sprite = _textureManager->createSprite(spriteInfo.path);
    if (!_sprite) {
        throw GraphicalEntityError("Failed to create sprite for: " + spriteName);
    }

    try {
        _animationManager = entityDrawing.createAnimationManager(spriteName);
        if (_animationManager) {
            const AnimationFrame &frame = _animationManager->getCurrentFrame();
            _sprite->setTextureRect(frame.x, frame.y, frame.width, frame.height);
        }
    } catch (const std::exception &) {
        if (spriteInfo.width > 0.0f && spriteInfo.height > 0.0f) {
            _sprite->setTextureRect(0, 0, static_cast<int>(spriteInfo.width), static_cast<int>(spriteInfo.height));
        }
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

float GraphicalEntity::getX() const
{
    return _x;
}

float GraphicalEntity::getY() const
{
    return _y;
}

const std::string &GraphicalEntity::getSpriteName() const
{
    return _spriteName;
}

float GraphicalEntity::getWidth() const
{
    SpriteInfo info = _entityDrawing.getSpriteInfoFromName(_spriteName);
    return info.width;
}

float GraphicalEntity::getHeight() const
{
    SpriteInfo info = _entityDrawing.getSpriteInfoFromName(_spriteName);
    return info.height;
}

void GraphicalEntity::render(const std::shared_ptr<IRenderer> &renderer)
{
    if (renderer && _sprite) {
        if (_animationManager) {
            try {
                const AnimationFrame &frame = _animationManager->getCurrentFrame();
                _sprite->setTextureRect(frame.x, frame.y, frame.width, frame.height);
            } catch (const SFMLAnimationManagerError &) {
            }
        }

        renderer->renderSprite(*_sprite);
    }
}

void GraphicalEntity::update(float deltaTime)
{
    if (_animationManager) {
        _animationManager->update(deltaTime);
    }
}

void GraphicalEntity::setAnimation(const std::string &animationName)
{
    if (!_animationManager) {
        throw GraphicalEntityError("No animation manager available for entity '" + _spriteName + "'");
    }

    try {
        _animationManager->setCurrentAnimation(animationName);
    } catch (const SFMLAnimationManagerError &e) {
        throw GraphicalEntityError(
            "Failed to set animation '" + animationName + "' for entity '" + _spriteName + "': " + e.what());
    }
}

std::shared_ptr<SFMLAnimationManager> GraphicalEntity::getAnimationManager() const
{
    return _animationManager;
}