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
        _text->setFont(fonts->load("fonts/r-type.otf"));
        _text->setString(label);
        _text->setCharacterSize(size == ButtonSize::Large ? 42 : 28);
        _text->setColor({255, 255, 255, 255});
    }

    void UIButton::setPosition(float x, float y)
    {
        _cmd.position = {x, y};

        if (!_text)
            return;

        const auto buttonW = static_cast<float>(_cmd.frame.w);
        const auto buttonH = static_cast<float>(_cmd.frame.h);
        const auto charSize = static_cast<unsigned int>(buttonH * 0.23f);
        _text->setCharacterSize(charSize);
        const auto bounds = dynamic_cast<const Graphics::SfmlText *>(_text.get())->get().getLocalBounds();
        const float textX = x + (buttonW - bounds.size.x) / 2.f - bounds.position.x;
        const float textY = y + (buttonH - bounds.size.y) / 2.f - bounds.position.y - 10.f;
        _text->setPosition(textX, textY);
    }

    void UIButton::update(float mouseX, float mouseY)
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

    bool UIButton::onMousePressed(float x, float y)
    {
        if (!bounds().contains(x, y))
            return false;
        _state = ButtonState::Pressed;
        return true;
    }

    bool UIButton::onMouseReleased(float x, float y)
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
        return _cmd.bounds();
    }

    void UIButton::reset()
    {
        _clicked = false;
        _state = ButtonState::Released;
    }
} // namespace Engine
