/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** Button
*/

#include "Button.hpp"
#include <utility>
#include "MenuScene.hpp"

using namespace Game;
using namespace Graphics;

Button::Button(float x, float y, const std::string &normalTexture, const std::string &hoverTexture,
    const std::string &pressedTexture, std::shared_ptr<ITextureManager> textureManager, const std::string &labelText,
    const std::string &fontPath)
    : _x(x), _y(y), _textureManager(std::move(textureManager))
{
    if (!_textureManager) {
        throw MenuSceneError("Texture manager cannot be null for Button");
    }

    if (!_textureManager->loadTexture(normalTexture)) {
        throw MenuSceneError("Failed to load button normal texture: " + normalTexture);
    }
    if (!_textureManager->loadTexture(hoverTexture)) {
        throw MenuSceneError("Failed to load button hover texture: " + hoverTexture);
    }
    if (!_textureManager->loadTexture(pressedTexture)) {
        throw MenuSceneError("Failed to load button pressed texture: " + pressedTexture);
    }

    _normalSprite = _textureManager->createSprite(normalTexture);
    _hoverSprite = _textureManager->createSprite(hoverTexture);
    _pressedSprite = _textureManager->createSprite(pressedTexture);

    if (!_normalSprite || !_hoverSprite || !_pressedSprite) {
        throw MenuSceneError("Failed to create button sprites");
    }

    _normalSprite->setPosition(_x, _y);
    _hoverSprite->setPosition(_x, _y);
    _pressedSprite->setPosition(_x, _y);

    _normalSprite->setScale(4.0f, 4.0f);
    _hoverSprite->setScale(4.0f, 4.0f);
    _pressedSprite->setScale(4.0f, 4.0f);

    _width = _normalSprite->getWidth();
    _height = _normalSprite->getHeight();

    if (!labelText.empty() && !fontPath.empty()) {
        _label = _textureManager->createText(fontPath, labelText, 30);
        if (_label) {
            float textX = _x + (_width - _label->getWidth()) / 2.0f;
            float textY = _y + (_height - _label->getHeight()) / 2.0f - 5.0f;
            _label->setPosition(textX, textY);
            _label->setColor(255, 255, 255, 255);
        }
    }
}

bool Button::update(float mouseX, float mouseY, bool isClicked)
{
    _previousState = _state;
    bool isHovering = contains(mouseX, mouseY);

    if (isHovering) {
        if (isClicked) {
            _state = ButtonState::Pressed;
            if (!_wasPressed) {
                _wasPressed = true;
            }
        } else {
            if (_wasPressed) {
                _wasPressed = false;
                if (_onClick) {
                    _onClick();
                }
                _state = ButtonState::Hover;
                return true;
            }
            _state = ButtonState::Hover;
        }
    } else {
        _state = ButtonState::Normal;
        if (!isClicked) {
            _wasPressed = false;
        }
    }

    return false;
}

void Button::render(const std::shared_ptr<IRenderer> &renderer)
{
    if (!renderer) {
        return;
    }

    switch (_state) {
        case ButtonState::Normal:
            if (_normalSprite) {
                renderer->renderSprite(*_normalSprite);
            }
            break;
        case ButtonState::Hover:
            if (_hoverSprite) {
                renderer->renderSprite(*_hoverSprite);
            }
            break;
        case ButtonState::Pressed:
            if (_pressedSprite) {
                renderer->renderSprite(*_pressedSprite);
            }
            break;
    }

    if (_label) {
        renderer->renderText(*_label);
    }
}

void Button::setOnClick(std::function<void()> callback)
{
    _onClick = std::move(callback);
}

float Button::getX() const
{
    return _x;
}

float Button::getY() const
{
    return _y;
}

float Button::getWidth() const
{
    return _width;
}

float Button::getHeight() const
{
    return _height;
}

bool Button::contains(float x, float y) const
{
    return x >= _x && x <= _x + _width && y >= _y && y <= _y + _height;
}
