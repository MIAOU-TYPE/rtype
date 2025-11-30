/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLSpriteManagement
*/

#include "SFMLSpriteManagement.hpp"

#include <utility>

using namespace Graphics;

SFMLSprite::SFMLSprite(sf::Sprite sprite) : _sprite(std::move(sprite))
{
}

void SFMLSprite::setPosition(float x, float y)
{
    _sprite.setPosition(x, y);
}

float SFMLSprite::getWidth() const
{
    return _sprite.getGlobalBounds().width;
}

float SFMLSprite::getHeight() const
{
    return _sprite.getGlobalBounds().height;
}

void SFMLSprite::setScale(float scaleX, float scaleY)
{
    _sprite.setScale(scaleX, scaleY);
}

void SFMLSprite::setTextureRect(int left, int top, int width, int height)
{
    _sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

const sf::Sprite &SFMLSprite::getSFMLSprite() const
{
    return _sprite;
}