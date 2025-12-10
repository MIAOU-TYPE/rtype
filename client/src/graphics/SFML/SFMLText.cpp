/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLText
*/

#include "SFMLText.hpp"

using namespace Graphics;

SFMLText::SFMLText(std::shared_ptr<sf::Font> font, const std::string &text, unsigned int characterSize)
    : _font(font), _text(*font, text, characterSize)
{
    _text.setFillColor(sf::Color::White);
}

void SFMLText::setString(const std::string &text)
{
    _text.setString(text);
}

void SFMLText::setCharacterSize(unsigned int size)
{
    _text.setCharacterSize(size);
}

void SFMLText::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    _text.setFillColor(sf::Color(r, g, b, a));
}

void SFMLText::setPosition(float x, float y)
{
    _text.setPosition({x, y});
}

float SFMLText::getWidth() const
{
    return _text.getGlobalBounds().size.x;
}

float SFMLText::getHeight() const
{
    return _text.getGlobalBounds().size.y;
}

const sf::Text &SFMLText::getSFMLText() const
{
    return _text;
}
