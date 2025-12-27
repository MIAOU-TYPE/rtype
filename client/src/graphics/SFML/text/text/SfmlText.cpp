/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SfmlText
*/

#include "SfmlText.hpp"

namespace Graphics
{
    SfmlText::SfmlText(const FontHandle font, const std::shared_ptr<SfmlFontManager> &fontManager)
        : _fontManager(fontManager), _color({255, 255, 255, 255}), _characterSize(30), _position({0.f, 0.f})
    {
        if (!_fontManager || !_fontManager->isValid(font))
            return;
        _text = std::make_unique<sf::Text>(_fontManager->get(font), "", _characterSize);
    }

    void SfmlText::setString(const std::string &text)
    {
        _content = text;
        if (_text)
            _text->setString(text);
    }

    void SfmlText::setFont(const FontHandle font)
    {
        if (!_fontManager || !_fontManager->isValid(font))
            return;

        const sf::Font &sfFont = _fontManager->get(font);

        if (!_text) {
            _text = std::make_unique<sf::Text>(sfFont, _content, _characterSize);
            _text->setFillColor(sf::Color(_color.r, _color.g, _color.b, _color.a));
            _text->setPosition({_position.first, _position.second});
        } else
            _text->setFont(sfFont);
    }

    void SfmlText::setCharacterSize(const unsigned int size)
    {
        _characterSize = size;
        if (_text)
            _text->setCharacterSize(size);
    }

    void SfmlText::setColor(const Color &color)
    {
        _color = color;
        if (_text)
            _text->setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    }

    void SfmlText::setPosition(const float x, const float y)
    {
        _position = {x, y};
        if (_text)
            _text->setPosition({x, y});
    }

    float SfmlText::getWidth() const
    {
        return _text ? _text->getLocalBounds().size.x : 0.f;
    }

    float SfmlText::getHeight() const
    {
        return _text ? _text->getLocalBounds().size.y : 0.f;
    }

    const sf::Text &SfmlText::get() const
    {
        if (!_text)
            throw TextError("SfmlText: Text object is not initialized.");
        return *_text;
    }
} // namespace Graphics
