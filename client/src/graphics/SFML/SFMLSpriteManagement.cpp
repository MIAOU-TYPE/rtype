/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLSpriteManagement
*/

#include "SFMLSpriteManagement.hpp"

using namespace Graphics;

SFMLSprite::SFMLSprite(const sf::Sprite &sprite) : _sprite(sprite)
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

const sf::Sprite &SFMLSprite::getSFMLSprite() const
{
    return _sprite;
}