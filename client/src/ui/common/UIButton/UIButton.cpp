/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UIButton
*/

#include "UIButton.hpp"

namespace UI
{
    UIButton::UIButton(
        const std::shared_ptr<Graphics::IRenderer> &renderer, const ButtonSize size, const std::string &label)
        : _renderer(renderer)
    {
        if (!_renderer)
            throw UIButtonError("UIButton: renderer is null");

        const auto textures = _renderer->textures();
        const std::string prefix = (size == ButtonSize::Large) ? "sprites/button_l_" : "sprites/button_s_";

        _released = textures->load(prefix + "released.png");
        _hover = textures->load(prefix + "hover.png");
        _pressed = textures->load(prefix + "pressed.png");

        if (_released == Graphics::InvalidTexture || _hover == Graphics::InvalidTexture
            || _pressed == Graphics::InvalidTexture) {
            throw UIButtonError("UIButton: failed to load button textures");
        }
        _baseSize = textures->getSize(_released);

        _cmd.textureId = _released;
        _cmd.frame = {0, 0, static_cast<int>(_baseSize.width), static_cast<int>(_baseSize.height)};
        _designScaleX = 1.7f;
        _designScaleY = 1.7f;
        _uiScaleX = 1.f;
        _uiScaleY = 1.f;
        _cmd.scale = {_designScaleX * _uiScaleX, _designScaleY * _uiScaleY};
        _text = _renderer->texts()->createText(size == ButtonSize::Large ? 42 : 34, {255, 255, 255, 255});
        if (_text)
            _text->setString(label);
        setPosition(0.f, 0.f);
    }

    void UIButton::applyStateVisual()
    {
        const auto textures = _renderer->textures();

        Graphics::TextureHandle tex = _released;
        switch (_state) {
            case ButtonState::Released: tex = _released; break;
            case ButtonState::Hover: tex = _hover; break;
            case ButtonState::Pressed: tex = _pressed; break;
        }

        _cmd.textureId = tex;
        const auto [width, height] = textures->getSize(tex);
        _cmd.frame = {0, 0, static_cast<int>(width), static_cast<int>(height)};

        const float baseW = static_cast<float>(_baseSize.width);
        const float baseH = static_cast<float>(_baseSize.height);
        const float curW = static_cast<float>(width);
        const float curH = static_cast<float>(height);

        const float offX = (baseW - curW) * 0.5f * _cmd.scale.x;
        const float offY = (baseH - curH) * 0.5f * _cmd.scale.y;

        _cmd.position = {_boxX + offX, _boxY + offY};
    }

    void UIButton::setPosition(const float x, const float y)
    {
        _boxX = x;
        _boxY = y;

        applyStateVisual();
        const float buttonW = static_cast<float>(_baseSize.width) * _cmd.scale.x;
        const float buttonH = static_cast<float>(_baseSize.height) * _cmd.scale.y;

        if (!_text)
            return;
        const auto charSize = static_cast<unsigned int>(buttonH * 0.45f);
        _text->setCharacterSize(charSize);
        const float textX = _boxX + (buttonW - _text->getWidth()) * 0.5f;
        const float textY = _boxY + (buttonH - _text->getHeight()) * 0.3f;
        _text->setPosition(textX, textY);
    }

    void UIButton::update(const float mouseX, const float mouseY)
    {
        const auto prev = _state;
        if (_state != ButtonState::Pressed)
            _state = bounds().contains(mouseX, mouseY) ? ButtonState::Hover : ButtonState::Released;
        if (_state != prev)
            applyStateVisual();
    }

    bool UIButton::onMousePressed(const float x, const float y)
    {
        if (!bounds().contains(x, y))
            return false;
        const auto prev = _state;
        _state = ButtonState::Pressed;
        if (_state != prev)
            applyStateVisual();
        return true;
    }

    bool UIButton::onMouseReleased(const float x, const float y)
    {
        const auto prev = _state;
        if (_state == ButtonState::Pressed && bounds().contains(x, y)) {
            _state = ButtonState::Hover;
            if (_state != prev)
                applyStateVisual();
            return true;
        }
        _state = ButtonState::Released;
        if (_state != prev)
            applyStateVisual();
        return false;
    }

    void UIButton::centerButtonLabel(
        const float centerX, const float y, Graphics::IText &label, const float labelCenterX) noexcept
    {
        setPosition(centerX - bounds().w * 0.5f, y);
        const auto b = bounds();
        label.setPosition(labelCenterX - label.getWidth() * 0.5f, b.y + (b.h - label.getHeight()) * 0.5f);
    }

    void UIButton::render() const
    {
        _renderer->draw(_cmd);
        if (_text)
            _renderer->draw(*_text);
    }

    Engine::FloatRect UIButton::bounds() const noexcept
    {
        return {_boxX, _boxY, static_cast<float>(_baseSize.width) * _cmd.scale.x,
            static_cast<float>(_baseSize.height) * _cmd.scale.y};
    }

    void UIButton::setLabel(const std::string &text)
    {
        if (_text)
            _text->setString(text);
        setPosition(_boxX, _boxY);
    }

    void UIButton::setScale(const float scaleX, const float scaleY)
    {
        _designScaleX = scaleX;
        _designScaleY = scaleY;
        _cmd.scale = {_designScaleX * _uiScaleX, _designScaleY * _uiScaleY};
        setPosition(_boxX, _boxY);
    }

    void UIButton::setUIScale(const float scaleX, const float scaleY)
    {
        _uiScaleX = scaleX;
        _uiScaleY = scaleY;
        _cmd.scale = {_designScaleX * _uiScaleX, _designScaleY * _uiScaleY};
        setPosition(_boxX, _boxY);
    }

    void UIButton::reset()
    {
        const auto prev = _state;
        _state = ButtonState::Released;
        if (_state != prev)
            applyStateVisual();
    }
} // namespace UI
