/*
** EPITECH PROJECT, 2025
** MenuState.hpp
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
        const auto textures = renderer->textures();
        const auto texts = renderer->texts();
        const auto fonts = renderer->fonts();
        const std::string prefix = (size == ButtonSize::Large) ? "sprites/button_l_" : "sprites/button_s_";

        _released = textures->load(prefix + "released.png");
        _hover = textures->load(prefix + "hover.png");
        _pressed = textures->load(prefix + "pressed.png");

        if (_released == Graphics::InvalidTexture || _hover == Graphics::InvalidTexture
            || _pressed == Graphics::InvalidTexture) {
            throw UIButtonError("Failed to load button textures");
        }

        _cmd.textureId = _released;
        _cmd.frame = {0, 0, static_cast<int>(textures->getSize(_released).width),
            static_cast<int>(textures->getSize(_released).height)};

        _text = _renderer->texts()->createText(size == ButtonSize::Large ? 42 : 28, {255, 255, 255, 255});
        _text->setString(label);
    }

    void UIButton::setPosition(const float x, const float y)
    {
        const float buttonW = static_cast<float>(_cmd.frame.w) * _cmd.scale.x;
        const float buttonH = static_cast<float>(_cmd.frame.h) * _cmd.scale.y;
        const auto charSize = static_cast<unsigned int>(buttonH * 0.45f);

        _cmd.position = {x, y};

        if (!_text)
            return;
        _text->setCharacterSize(charSize);
        const float textX = x + (buttonW - _text->getWidth()) / 2.f;
        const float textY = y + (buttonH - _text->getHeight()) / 2.f - 25.f;
        _text->setPosition(textX, textY);
    }

    void UIButton::update(const float mouseX, const float mouseY)
    {
        if (_state != ButtonState::Pressed)
            _state = bounds().contains(mouseX, mouseY) ? ButtonState::Hover : ButtonState::Released;
        switch (_state) {
            case ButtonState::Released: _cmd.textureId = _released; break;
            case ButtonState::Hover: _cmd.textureId = _hover; break;
            case ButtonState::Pressed: _cmd.textureId = _pressed; break;
        }
    }

    bool UIButton::onMousePressed(const float x, const float y)
    {
        if (!bounds().contains(x, y))
            return false;
        _state = ButtonState::Pressed;
        return true;
    }

    bool UIButton::onMouseReleased(const float x, const float y)
    {
        if (_state == ButtonState::Pressed && bounds().contains(x, y)) {
            _state = ButtonState::Hover;
            return true;
        }
        _state = ButtonState::Released;
        return false;
    }

    void UIButton::centerButtonLabel(float centerX, float y, Graphics::IText &label, float labelCenterX)
    {
        setPosition(centerX - bounds().w * 0.5f, y);
        const auto b = bounds();
        label.setPosition(labelCenterX - label.getWidth() * 0.5f, b.y + (b.h - label.getHeight()) * 0.5f);
    }

    void UIButton::render() const
    {
        _renderer->draw(_cmd);
        _renderer->draw(*_text);
    }

    Engine::FloatRect UIButton::bounds() const noexcept
    {
        return {_cmd.position.x, _cmd.position.y, static_cast<float>(_cmd.frame.w) * _cmd.scale.x,
            static_cast<float>(_cmd.frame.h) * _cmd.scale.y};
    }

    void UIButton::setLabel(const std::string &text) const
    {
        if (_text)
            _text->setString(text);
    }

    void UIButton::setScale(const float scale)
    {
        _cmd.scale = {scale, scale};
    }

    void UIButton::reset()
    {
        _state = ButtonState::Released;
    }
} // namespace UI
