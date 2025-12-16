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
#include "SFMLText.hpp"

using namespace Graphics;

void SFMLRenderer::createWindow(unsigned int width, unsigned int height, const std::string &title)
{
    _window.create(sf::VideoMode({width, height}), title, sf::Style::Close | sf::Style::Titlebar);
    _window.setPosition(sf::Vector2i(0, 0));
}

bool SFMLRenderer::isOpen() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _window.isOpen();
}

void SFMLRenderer::close()
{
    std::lock_guard<std::mutex> lock(_mutex);
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
    std::lock_guard<std::mutex> lock(_mutex);
    if (auto opt = _window.pollEvent()) {
        event = std::make_shared<SFMLEvent>(*opt);

        if (event->isType(EventType::MouseButtonPressed)) {
            _isMousePressed = true;
        } else if (event->isType(EventType::MouseButtonReleased)) {
            _isMousePressed = false;
        }

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

void SFMLRenderer::renderText(const IText &text)
{
    const auto *sfmlText = dynamic_cast<const SFMLText *>(&text);
    if (sfmlText) {
        _window.draw(sfmlText->getSFMLText());
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

void SFMLRenderer::getMousePosition(float &x, float &y) const
{
    auto mousePos = sf::Mouse::getPosition(_window);
    x = static_cast<float>(mousePos.x);
    y = static_cast<float>(mousePos.y);
}

float SFMLRenderer::getElapsedTime() const
{
    return _clock.getElapsedTime().asSeconds();
}

void SFMLRenderer::restartClock()
{
    _clock.restart();
}

void SFMLRenderer::setIsMousePressed(bool isPressed)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _isMousePressed = isPressed;
}

bool SFMLRenderer::getIsMousePressed() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _isMousePressed;
}

void SFMLRenderer::setActive(bool active)
{
    std::lock_guard<std::mutex> lock(_mutex);
    (void) _window.setActive(active);
}
