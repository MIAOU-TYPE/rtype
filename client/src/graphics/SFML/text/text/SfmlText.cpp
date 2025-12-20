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
        : _fontManager(fontManager), _color({255, 255, 255, 255})
    {
        if (!_fontManager->isValid(font))
            return;
        _text = sf::Text(fontManager->get(font), "", 30);
    }

    void SfmlText::setString(const std::string &text)
    {
        if (_text)
            _text->setString(text);
        _content = text;
    }

    void SfmlText::setFont(const FontHandle font)
    {
        if (!_fontManager->isValid(font))
            return;

        const sf::Font &sfFont = _fontManager->get(font);

        if (_text)
            _text->setFont(sfFont);
        else {
            _text = sf::Text(sfFont, _content, _characterSize);
            setColor(_color);
            setPosition(_position.first, _position.second);
        }
    }

    void SfmlText::setCharacterSize(const unsigned int size)
    {
        if (_text)
            _text->setCharacterSize(size);
        _characterSize = size;
    }

    void SfmlText::setColor(const Color &color)
    {
        if (_text)
            _text->setFillColor(sf::Color(color.r, color.g, color.b, color.a));
        _color = color;
    }

    void SfmlText::setPosition(const float x, const float y)
    {
        if (_text)
            _text->setPosition({x, y});
        _position = {x, y};
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
        return _text.value();
    }
} // namespace Graphics
