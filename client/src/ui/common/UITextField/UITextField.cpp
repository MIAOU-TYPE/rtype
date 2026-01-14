/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UITextField
*/

#include "UITextField.hpp"

namespace
{
    std::string maskPassword(const std::string &s)
    {
        return std::string(s.size(), '*');
    }
} // namespace

namespace UI
{
    UITextField::UITextField(std::shared_ptr<Graphics::IRenderer> renderer, std::string label, bool password)
        : _renderer(std::move(renderer)), _label(std::move(label)), _password(password)
    {
        _labelText = _renderer->texts()->createText(26, {255, 255, 255, 255});
        _valueText = _renderer->texts()->createText(28, {220, 220, 220, 255});
        _labelText->setString(_label);
        _valueText->setString("");
        layout();
    }

    void UITextField::setPosition(float x, float y)
    {
        _x = x;
        _y = y;
        layout();
    }

    void UITextField::setWidth(float w)
    {
        _w = w;
        layout();
    }

    void UITextField::layout()
    {
        if (!_labelText || !_valueText)
            return;
        _labelText->setPosition(_x, _y);
        _valueText->setPosition(_x, _y + 32.f);
        _h = 32.f + 34.f;
    }

    void UITextField::render() const
    {
        if (!_renderer || !_labelText || !_valueText)
            return;
        _renderer->draw(*_labelText);
        _renderer->draw(*_valueText);
    }

    void UITextField::clear()
    {
        _value.clear();
        if (_valueText)
            _valueText->setString("");
    }

    void UITextField::setFocused(const bool v)
    {
        _focused = v;
        if (_labelText)
            _labelText->setString(_focused ? ("> " + _label) : _label);
        if (_valueText) {
            const std::string shown = _password ? maskPassword(_value) : _value;
            _valueText->setString(_focused ? (shown + "|") : shown);
        }
    }

    bool UITextField::isFocused() const noexcept
    {
        return _focused;
    }

    const std::string &UITextField::value() const noexcept
    {
        return _value;
    }

    void UITextField::setValue(std::string v)
    {
        _value = std::move(v);
        if (_value.size() > _maxLen)
            _value.resize(_maxLen);
        if (_valueText) {
            const std::string shown = _password ? maskPassword(_value) : _value;
            _valueText->setString(_focused ? (shown + "|") : shown);
        }
    }

    Rect UITextField::bounds() const noexcept
    {
        return Rect{_x, _y, _w, _h};
    }

    void UITextField::onMousePressed(const float mx, const float my)
    {
        setFocused(bounds().contains(mx, my));
    }

    void UITextField::onKeyPressed(const Engine::Key key)
    {
        if (!_focused)
            return;
        if (key == Engine::Key::Backspace) {
            if (!_value.empty())
                _value.pop_back();
            setValue(_value);
            return;
        }
        char c = 0;
        if (!keyToChar(key, c))
            return;
        if (_value.size() >= _maxLen)
            return;
        _value.push_back(c);
        setValue(_value);
    }

    bool UITextField::keyToChar(const Engine::Key key, char &out) noexcept
    {
        switch (key) {
            case Engine::Key::A: out = 'a'; return true;
            case Engine::Key::B: out = 'b'; return true;
            case Engine::Key::C: out = 'c'; return true;
            case Engine::Key::D: out = 'd'; return true;
            case Engine::Key::E: out = 'e'; return true;
            case Engine::Key::F: out = 'f'; return true;
            case Engine::Key::G: out = 'g'; return true;
            case Engine::Key::H: out = 'h'; return true;
            case Engine::Key::I: out = 'i'; return true;
            case Engine::Key::J: out = 'j'; return true;
            case Engine::Key::K: out = 'k'; return true;
            case Engine::Key::L: out = 'l'; return true;
            case Engine::Key::M: out = 'm'; return true;
            case Engine::Key::N: out = 'n'; return true;
            case Engine::Key::O: out = 'o'; return true;
            case Engine::Key::P: out = 'p'; return true;
            case Engine::Key::Q: out = 'q'; return true;
            case Engine::Key::R: out = 'r'; return true;
            case Engine::Key::S: out = 's'; return true;
            case Engine::Key::T: out = 't'; return true;
            case Engine::Key::U: out = 'u'; return true;
            case Engine::Key::V: out = 'v'; return true;
            case Engine::Key::W: out = 'w'; return true;
            case Engine::Key::X: out = 'x'; return true;
            case Engine::Key::Y: out = 'y'; return true;
            case Engine::Key::Z: out = 'z'; return true;
            case Engine::Key::Space: out = ' '; return true;
            case Engine::Key::Num0: out = '0'; return true;
            case Engine::Key::Num1: out = '1'; return true;
            case Engine::Key::Num2: out = '2'; return true;
            case Engine::Key::Num3: out = '3'; return true;
            case Engine::Key::Num4: out = '4'; return true;
            case Engine::Key::Num5: out = '5'; return true;
            case Engine::Key::Num6: out = '6'; return true;
            case Engine::Key::Num7: out = '7'; return true;
            case Engine::Key::Num8: out = '8'; return true;
            case Engine::Key::Num9: out = '9'; return true;
            default: break;
        }
        return false;
    }
} // namespace UI
