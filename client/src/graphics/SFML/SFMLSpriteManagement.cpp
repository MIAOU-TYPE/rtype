/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLSpriteManagement
*/

#include "SFMLSpriteManagement.hpp"

using namespace Graphics;

SFMLSprite::SFMLSprite(sf::Sprite sprite) : _sprite(std::move(sprite))
{
}

void SFMLSprite::setPosition(float x, float y)
{
    _sprite.setPosition(sf::Vector2f{x, y});
}

float SFMLSprite::getWidth() const
{
    const auto bounds = _sprite.getGlobalBounds();
    return bounds.size.x;
}

float SFMLSprite::getHeight() const
{
    const auto bounds = _sprite.getGlobalBounds();
    return bounds.size.y;
}

void SFMLSprite::setScale(float scaleX, float scaleY)
{
    _sprite.setScale(sf::Vector2f{scaleX, scaleY});
}

void SFMLSprite::setTextureRect(int left, int top, int width, int height)
{
    sf::IntRect rect{sf::Vector2i{left, top}, sf::Vector2i{width, height}};
    _sprite.setTextureRect(rect);
}

void SFMLSprite::setTextureRect(int left, int top, int width, int height)
{
    _sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

const sf::Sprite &SFMLSprite::getSFMLSprite() const
{
    return _sprite;
}