/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLRenderer
*/

#include "SFMLRenderer.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <iostream>
#include "SFMLSpriteManagement.hpp"
#include "SFMLText.hpp"

using namespace Graphics;

SFMLRenderer::SFMLRenderer(std::shared_ptr<Resources::IResourceManager> resourceManager) : _resourceManager(std::move(resourceManager))
{
}

void SFMLRenderer::createWindow(unsigned int width, unsigned int height, const std::string &title)
{
    _window.create(sf::VideoMode({width, height}), title);
    if (!_renderTexture.resize({width, height}))
        throw std::runtime_error("Failed to create render texture for SFMLRenderer.");
    auto resource = _resourceManager->loadResource("shaders/colorblind.frag");

    if (resource.data) {
        std::string shaderCode(reinterpret_cast<const char*>(resource.data), resource.size);
        if (!_colorBlindShader.loadFromMemory(shaderCode, sf::Shader::Type::Fragment))
            _shaderLoaded = false;
        else {
            _shaderLoaded = true;
            _colorBlindShader.setUniform("texture", sf::Shader::CurrentTexture);
            _colorBlindShader.setUniform("colorBlindMode", 0);
        }
    } else
        _shaderLoaded = false;
    if (_shaderLoaded)
        _currentTarget = &_renderTexture;
    else
        _currentTarget = &_window;
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
    if (_currentTarget) {
        _currentTarget->clear();
    }
}

void SFMLRenderer::display()
{
    if (_shaderLoaded) {
        _renderTexture.display();
        sf::Sprite sprite(_renderTexture.getTexture());
        _window.clear();
        if (_colorBlindMode != ColorBlindType::NONE)
            _window.draw(sprite, &_colorBlindShader);
        else
            _window.draw(sprite);
    }
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
    if (_currentTarget) {
        _currentTarget->draw(sprite);
    }
}

void SFMLRenderer::renderSprite(const ISprite &sprite)
{
    const auto *sfmlSprite = dynamic_cast<const SFMLSprite *>(&sprite);
    if (sfmlSprite && _currentTarget) {
        auto pos = sfmlSprite->getSFMLSprite().getPosition();
        std::cout << "pos x: " << pos.x << " pos y: " << pos.y << std::endl;
        _currentTarget->draw(sfmlSprite->getSFMLSprite());
    }
}

void SFMLRenderer::renderText(const IText &text)
{
    const auto *sfmlText = dynamic_cast<const SFMLText *>(&text);
    if (sfmlText && _currentTarget) {
        _currentTarget->draw(sfmlText->getSFMLText());
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

void SFMLRenderer::setColorBlindMode(ColorBlindType type)
{
    _colorBlindMode = type;

    if (_shaderLoaded) {
        _colorBlindShader.setUniform("colorBlindMode", static_cast<int>(type));
    }
}

ColorBlindType SFMLRenderer::getColorBlindMode() const
{
    return _colorBlindMode;
}