/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLInputHandler
*/

#include "SFMLInputHandler.hpp"
#include <utility>

using namespace Input;
using namespace Events;

SFMLInputHandler::SFMLInputHandler(std::shared_ptr<Events::InputEventManager> eventManager)
    : _eventManager(std::move(eventManager))
{
    if (!_eventManager) {
        throw SFMLInputHandlerError("InputEventManager cannot be null");
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
    _keyMappings[sf::Keyboard::Key::Z] = InputAction::MoveUp;
    _keyMappings[sf::Keyboard::Key::Up] = InputAction::MoveUp;

    _keyMappings[sf::Keyboard::Key::S] = InputAction::MoveDown;
    _keyMappings[sf::Keyboard::Key::Down] = InputAction::MoveDown;

    _keyMappings[sf::Keyboard::Key::Q] = InputAction::MoveLeft;
    _keyMappings[sf::Keyboard::Key::Left] = InputAction::MoveLeft;

    _keyMappings[sf::Keyboard::Key::D] = InputAction::MoveRight;
    _keyMappings[sf::Keyboard::Key::Right] = InputAction::MoveRight;

    _keyMappings[sf::Keyboard::Key::Space] = InputAction::Shoot;
    _keyMappings[sf::Keyboard::Key::LControl] = InputAction::Shoot;

    _keyMappings[sf::Keyboard::Key::Escape] = InputAction::Quit;
    _keyMappings[sf::Keyboard::Key::P] = InputAction::Pause;
    _keyMappings[sf::Keyboard::Key::Enter] = InputAction::Confirm;
    _keyMappings[sf::Keyboard::Key::Backspace] = InputAction::Cancel;
}

void SFMLInputHandler::handleKeyboardEvent(const sf::Event &event)
{
    const sf::Event::KeyPressed *keyPressed = event.getIf<sf::Event::KeyPressed>();
    const sf::Event::KeyReleased *keyReleased = event.getIf<sf::Event::KeyReleased>();

    if (!keyPressed && !keyReleased) {
        return;
    }

    sf::Keyboard::Key keyCode;
    bool isPressed;

    if (keyPressed) {
        keyCode = keyPressed->code;
        isPressed = true;
    } else {
        keyCode = keyReleased->code;
        isPressed = false;
    }

    auto it = _keyMappings.find(keyCode);
    if (it == _keyMappings.end()) {
        return;
    }

    if (isPressed) {
        if (_pressedKeys.find(keyCode) != _pressedKeys.end()) {
            return;
        }
        _pressedKeys.insert(keyCode);
    } else {
        _pressedKeys.erase(keyCode);
    }

    InputEvent inputEvent;
    inputEvent.action = it->second;

    if (isPressed) {
        inputEvent.state = InputState::Pressed;
    } else {
        inputEvent.state = InputState::Released;
    }

    _eventManager->dispatchEvent(inputEvent);
}