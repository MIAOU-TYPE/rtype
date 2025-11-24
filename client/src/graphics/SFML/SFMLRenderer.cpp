/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#include "SFMLRenderer.hpp"

using namespace Graphics;

void SFMLRenderer::createWindow(unsigned int width, unsigned int height, const std::string &title)
{
    window.create(sf::VideoMode(width, height), title);
}

bool SFMLRenderer::isOpen() const
{
    return window.isOpen();
}

void SFMLRenderer::close()
{
    window.close();
}

void SFMLRenderer::clear()
{
    window.clear();
}

void SFMLRenderer::display()
{
    window.display();
}

bool SFMLRenderer::pollEvent(sf::Event &event)
{
    return window.pollEvent(event);
}

bool SFMLRenderer::isWindowCloseEvent(const sf::Event &event) const
{
    return event.type == sf::Event::Closed;
}