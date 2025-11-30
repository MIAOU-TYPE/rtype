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
    auto bounds = _sprite.getGlobalBounds();
    return bounds.size.x;
}

float SFMLSprite::getHeight() const
{
    auto bounds = _sprite.getGlobalBounds();
    return bounds.size.y;
}

void SFMLSprite::setScale(float scaleX, float scaleY)
{
    _sprite.setScale(sf::Vector2f{scaleX, scaleY});
}

const sf::Sprite &SFMLSprite::getSFMLSprite() const
{
    return _sprite;
}