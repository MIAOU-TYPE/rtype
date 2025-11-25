/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Starfield
*/

#include "Starfield.hpp"

using namespace Background;

Starfield::Starfield(
    std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<Graphics::ITextureManager> textureManager)
    : _renderer(renderer), _textureManager(textureManager)
{
    if (!_renderer) {
        throw StarfieldError("Renderer cannot be null");
    }

    if (!_textureManager) {
        throw StarfieldError("Texture manager cannot be null");
    }

    if (!_textureManager->loadTexture(BACKGROUND_TEXTURE_PATH)) {
        throw StarfieldError("Failed to load background texture: " + BACKGROUND_TEXTURE_PATH);
    }

    _sprite1 = _textureManager->createSprite(BACKGROUND_TEXTURE_PATH);
    _sprite2 = _textureManager->createSprite(BACKGROUND_TEXTURE_PATH);

    // _backgroundWidth = _sprite1->getWidth();

    unsigned int windowHeight = _renderer->getWindowHeight();
    float spriteHeight = _sprite1->getHeight();
    _scaleY = static_cast<float>(windowHeight) / spriteHeight;

    _sprite1->setScale(_scaleY, _scaleY);
    _sprite2->setScale(_scaleY, _scaleY);

    _backgroundWidth = _sprite1->getWidth();

    _background1X = 0.0f;
    _background2X = _backgroundWidth;
}

void Starfield::update(float deltaTime)
{
    _background1X -= _scrollSpeed * deltaTime;
    _background2X -= _scrollSpeed * deltaTime;

    if (_background1X <= -_backgroundWidth) {
        _background1X = _background2X + _backgroundWidth;
    }
    if (_background2X <= -_backgroundWidth) {
        _background2X = _background1X + _backgroundWidth;
    }
}

void Starfield::render()
{
    try {
        if (_sprite1) {
            _sprite1->setPosition(_background1X, 0.0f);
            _renderer->renderSprite(*_sprite1);
        }

        if (_sprite2) {
            _sprite2->setPosition(_background2X, 0.0f);
            _renderer->renderSprite(*_sprite2);
        }
    } catch (const std::exception &e) {
        throw StarfieldError("Failed to render starfield: " + std::string(e.what()));
    }
}
