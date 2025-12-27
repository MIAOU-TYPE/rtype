/*
** EPITECH PROJECT, 2025
** MenuState.hpp
** File description:
** UIButton
*/

#include "UIButton.hpp"

namespace Engine
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
        _cmd.textureId = _released;

        const auto sizeTex = textures->getSize(_released);
        _cmd.frame = {0, 0, static_cast<int>(sizeTex.width), static_cast<int>(sizeTex.height)};

        _text = texts->createText();
        _text->setFont(fonts->load("fonts/font.ttf"));
        _text->setString(label);
        _text->setCharacterSize(size == ButtonSize::Large ? 42 : 28);
        _text->setColor({255, 255, 255, 255});
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
        const bool hovered = bounds().contains(mouseX, mouseY);

        if (_state != ButtonState::Pressed)
            _state = hovered ? ButtonState::Hover : ButtonState::Released;
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
            _clicked = true;
            _state = ButtonState::Hover;
            return true;
        }
        _state = ButtonState::Released;
        return false;
    }

    bool UIButton::wasClicked() const noexcept
    {
        return _clicked;
    }

    void UIButton::render() const
    {
        _renderer->draw(_cmd);
        _renderer->drawText(*_text);
    }

    FloatRect UIButton::bounds() const noexcept
    {
        return {_cmd.position.x, _cmd.position.y, _cmd.frame.w * _cmd.scale.x, _cmd.frame.h * _cmd.scale.y};
    }

    void UIButton::setLabel(const std::string &text) const
    {
        if (_text)
            _text->setString(text);
    }

    void UIButton::setScale(float scale)
    {
        _scale = scale * 0.85f;
        _cmd.scale = {scale, scale};
    }

    void UIButton::reset()
    {
        _clicked = false;
        _state = ButtonState::Released;
    }
} // namespace Engine
