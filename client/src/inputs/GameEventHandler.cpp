/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameEventHandler
*/

#include "GameEventHandler.hpp"
#include <iostream>
#include <utility>
#include "InputComponent.hpp"

using namespace Input;
using namespace Events;

GameEventHandler::GameEventHandler(std::shared_ptr<Game::GameScene> gameScene) : _gameScene(std::move(gameScene))
{
}

void GameEventHandler::setQuitCallback(std::function<void()> callback)
{
    _quitCallback = std::move(callback);
}

void GameEventHandler::setPlayerEntity(Ecs::Entity entity)
{
    _playerEntity = entity;
}

void GameEventHandler::onInputEvent(const InputEvent &event)
{
    switch (event.action) {
        case InputAction::MoveUp:
        case InputAction::MoveDown:
        case InputAction::MoveLeft:
        case InputAction::MoveRight: handleMovement(event.action, event.state); break;

        case InputAction::Shoot: handleShooting(event.state); break;

        case InputAction::Pause:
        case InputAction::Quit:
        case InputAction::Confirm:
        case InputAction::Cancel: handleSystemActions(event.action, event.state); break;
    }
}

void GameEventHandler::handleMovement(InputAction action, InputState state)
{
    if (!_gameScene)
        return;

    auto &registry = _gameScene->getRegistry();
    auto &inputs = registry.getComponents<Ecs::InputComponent>();
    auto &input = inputs[static_cast<size_t>(_playerEntity)];

    if (!input.has_value())
        return;

    // bool isPressed = (state == InputState::Pressed || state == InputState::Held);

    switch (action) {
        case InputAction::MoveUp: std::cout << "Move up pressed" ; break;
        case InputAction::MoveDown:  std::cout << "Move up pressed"; break;
        case InputAction::MoveLeft:  std::cout << "Move up pressed"; break;
        case InputAction::MoveRight:  std::cout << "Move up pressed"; break;
        default: break;
    }
}

void GameEventHandler::handleShooting(InputState state)
{
    if (!_gameScene) {
        return;
    }

    auto &registry = _gameScene->getRegistry();
    auto &inputs = registry.getComponents<Ecs::InputComponent>();
    auto &input = inputs[static_cast<size_t>(_playerEntity)];

    if (!input.has_value()) {
        return;
    }

    input->shoot = (state == InputState::Pressed || state == InputState::Held);
}

void GameEventHandler::handleSystemActions(InputAction action, InputState state)
{
    if (state != InputState::Pressed) {
        return;
    }

    switch (action) {
        case InputAction::Pause: std::cout << "Game paused/unpaused" << std::endl; break;
        case InputAction::Quit:
            std::cout << "Quit game requested" << std::endl;
            if (_quitCallback) {
                _quitCallback();
            }
            break;
        case InputAction::Confirm: std::cout << "Confirm action" << std::endl; break;
        case InputAction::Cancel: std::cout << "Cancel action" << std::endl; break;
        default: break;
    }
}