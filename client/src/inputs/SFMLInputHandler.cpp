/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#include "SFMLInputHandler.hpp"

using namespace Input;
using namespace Events;

SFMLInputHandler::SFMLInputHandler(std::shared_ptr<Events::InputEventManager> eventManager)
    : _eventManager(eventManager)
{
    if (!_eventManager) {
        throw std::runtime_error("InputEventManager cannot be null");
    }
    loadDefaultMappings();
}

void SFMLInputHandler::handleEvent(const sf::Event &event)
{
    handleKeyboardEvent(event);
}

void SFMLInputHandler::update(float deltaTime)
{
    _eventManager->updateHeldActions(deltaTime);
}

void SFMLInputHandler::mapKey(sf::Keyboard::Key key, Events::InputAction action)
{
    _keyMappings[key] = action;
}

void SFMLInputHandler::unmapKey(sf::Keyboard::Key key)
{
    _keyMappings.erase(key);
}

bool SFMLInputHandler::isMapped(sf::Keyboard::Key key) const
{
    return _keyMappings.find(key) != _keyMappings.end();
}

bool SFMLInputHandler::isKeyPressed(sf::Keyboard::Key key) const
{
    return sf::Keyboard::isKeyPressed(key);
}

void SFMLInputHandler::loadDefaultMappings()
{
    _keyMappings[sf::Keyboard::Z] = InputAction::MoveUp;
    _keyMappings[sf::Keyboard::Up] = InputAction::MoveUp;

    _keyMappings[sf::Keyboard::S] = InputAction::MoveDown;
    _keyMappings[sf::Keyboard::Down] = InputAction::MoveDown;

    _keyMappings[sf::Keyboard::Q] = InputAction::MoveLeft;
    _keyMappings[sf::Keyboard::Left] = InputAction::MoveLeft;

    _keyMappings[sf::Keyboard::D] = InputAction::MoveRight;
    _keyMappings[sf::Keyboard::Right] = InputAction::MoveRight;

    _keyMappings[sf::Keyboard::Space] = InputAction::Shoot;
    _keyMappings[sf::Keyboard::LControl] = InputAction::Shoot;

    _keyMappings[sf::Keyboard::Escape] = InputAction::Quit;
    _keyMappings[sf::Keyboard::P] = InputAction::Pause;
    _keyMappings[sf::Keyboard::Enter] = InputAction::Confirm;
    _keyMappings[sf::Keyboard::Backspace] = InputAction::Cancel;
}

void SFMLInputHandler::handleKeyboardEvent(const sf::Event &event)
{
    if (event.type != sf::Event::KeyPressed && event.type != sf::Event::KeyReleased) {
        return;
    }

    auto it = _keyMappings.find(event.key.code);
    if (it == _keyMappings.end()) {
        return;
    }

    InputEvent inputEvent;
    inputEvent.action = it->second;

    if (event.type == sf::Event::KeyPressed) {
        inputEvent.state = InputState::Pressed;
    } else if (event.type == sf::Event::KeyReleased) {
        inputEvent.state = InputState::Released;
    }

    _eventManager->dispatchEvent(inputEvent);
}