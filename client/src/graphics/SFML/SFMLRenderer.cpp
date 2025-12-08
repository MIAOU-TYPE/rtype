/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#include "SFMLRenderer.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "SFMLSpriteManagement.hpp"

using namespace Graphics;

void SFMLRenderer::createWindow(unsigned int width, unsigned int height, const std::string &title)
{
    _window.create(sf::VideoMode({width, height}), title);
}

bool SFMLRenderer::isOpen() const
{
    return _window.isOpen();
}

void SFMLRenderer::close()
{
    _window.close();
}

void SFMLRenderer::clear()
{
    _window.clear();
}

void SFMLRenderer::display()
{
    _window.display();
}

bool SFMLRenderer::pollEvent(std::shared_ptr<IEvent> &event)
{
    if (auto opt = _window.pollEvent()) {
        event = std::make_shared<SFMLEvent>(*opt);
        return true;
    }
    return false;
}

bool SFMLRenderer::isWindowCloseEvent(const IEvent &event) const
{
    return event.isType(EventType::Closed);
}

void SFMLRenderer::drawSprite(const sf::Sprite &sprite)
{
    _window.draw(sprite);
}

void SFMLRenderer::renderSprite(const ISprite &sprite)
{
    const auto *sfmlSprite = dynamic_cast<const SFMLSprite *>(&sprite);
    if (sfmlSprite) {
        _window.draw(sfmlSprite->getSFMLSprite());
    }
}

unsigned int SFMLRenderer::getWindowWidth() const
{
    return _window.getSize().x;
}

unsigned int SFMLRenderer::getWindowHeight() const
{
    return _window.getSize().y;
}
