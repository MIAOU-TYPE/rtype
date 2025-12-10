/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLEvent
*/

#include "SFMLEvent.hpp"

using namespace Graphics;

SFMLEvent::SFMLEvent(const sf::Event &sfEvent) : _sfmlEvent(sfEvent)
{
}

EventType SFMLEvent::getType() const
{
    return convertEventType();
}

bool SFMLEvent::isType(EventType type) const
{
    return getType() == type;
}

const sf::Event &SFMLEvent::getSFMLEvent() const
{
    return _sfmlEvent;
}

EventType SFMLEvent::convertEventType() const
{
    if (_sfmlEvent.getIf<sf::Event::Closed>())
        return EventType::Closed;
    if (_sfmlEvent.getIf<sf::Event::Resized>())
        return EventType::Resized;
    if (_sfmlEvent.getIf<sf::Event::TextEntered>())
        return EventType::TextEntered;
    if (_sfmlEvent.getIf<sf::Event::KeyPressed>())
        return EventType::KeyPressed;
    if (_sfmlEvent.getIf<sf::Event::KeyReleased>())
        return EventType::KeyReleased;
    if (_sfmlEvent.getIf<sf::Event::MouseWheelScrolled>())
        return EventType::MouseWheelMoved;
    if (_sfmlEvent.getIf<sf::Event::MouseButtonPressed>())
        return EventType::MouseButtonPressed;
    if (_sfmlEvent.getIf<sf::Event::MouseButtonReleased>())
        return EventType::MouseButtonReleased;
    if (_sfmlEvent.getIf<sf::Event::MouseMoved>())
        return EventType::MouseMoved;
    if (_sfmlEvent.getIf<sf::Event::MouseEntered>())
        return EventType::MouseEntered;
    if (_sfmlEvent.getIf<sf::Event::MouseLeft>())
        return EventType::MouseLeft;
    if (_sfmlEvent.getIf<sf::Event::JoystickButtonPressed>())
        return EventType::JoystickButtonPressed;
    if (_sfmlEvent.getIf<sf::Event::JoystickButtonReleased>())
        return EventType::JoystickButtonReleased;
    if (_sfmlEvent.getIf<sf::Event::JoystickMoved>())
        return EventType::JoystickMoved;
    if (_sfmlEvent.getIf<sf::Event::JoystickConnected>())
        return EventType::JoystickConnected;
    if (_sfmlEvent.getIf<sf::Event::JoystickDisconnected>())
        return EventType::JoystickDisconnected;
    if (_sfmlEvent.getIf<sf::Event::TouchBegan>())
        return EventType::TouchBegan;
    if (_sfmlEvent.getIf<sf::Event::TouchMoved>())
        return EventType::TouchMoved;
    if (_sfmlEvent.getIf<sf::Event::TouchEnded>())
        return EventType::TouchEnded;
    if (_sfmlEvent.getIf<sf::Event::SensorChanged>())
        return EventType::SensorChanged;
    return EventType::Unknown;
}

bool SFMLEvent::isMouseButtonPressed() const
{
    return _sfmlEvent.getIf<sf::Event::MouseButtonPressed>() != nullptr;
}

bool SFMLEvent::isMouseButtonReleased() const
{
    return _sfmlEvent.getIf<sf::Event::MouseButtonReleased>() != nullptr;
}

bool SFMLEvent::getMousePosition(float &x, float &y) const
{
    if (const auto *mouseMoved = _sfmlEvent.getIf<sf::Event::MouseMoved>()) {
        x = static_cast<float>(mouseMoved->position.x);
        y = static_cast<float>(mouseMoved->position.y);
        return true;
    }
    return false;
}
