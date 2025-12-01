/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** GameEventHandler
*/

#include "GameEventHandler.hpp"
#include <iostream>
#include <utility>
#include "GameScene.hpp"

using namespace Input;
using namespace Events;

GameEventHandler::GameEventHandler(std::shared_ptr<Game::GameScene> gameScene) : _gameScene(std::move(gameScene))
{
}

void GameEventHandler::setQuitCallback(std::function<void()> callback)
{
    _quitCallback = std::move(callback);
}

void GameEventHandler::onInputEvent(const InputEvent &event)
{
    switch (event.action) {
        case InputAction::MoveUp:
        case InputAction::MoveDown:
        case InputAction::MoveLeft:
        case InputAction::MoveRight: handleMovement(event.action, event.state, event.deltaTime); break;

        case InputAction::Shoot: handleShooting(event.state); break;

        case InputAction::Pause:
        case InputAction::Quit:
        case InputAction::Confirm:
        case InputAction::Cancel: handleSystemActions(event.action, event.state); break;
    }
}

void GameEventHandler::handleMovement(InputAction action, InputState state, float /* deltaTime */)
{
    if (_gameScene) {
        if (state == InputState::Pressed) {
            switch (action) {
                case InputAction::MoveUp: std::cout << "Player starts moving up" << std::endl; break;
                case InputAction::MoveDown: std::cout << "Player starts moving down" << std::endl; break;
                case InputAction::MoveLeft: std::cout << "Player starts moving left" << std::endl; break;
                case InputAction::MoveRight: std::cout << "Player starts moving right" << std::endl; break;
                default: break;
            }
        } else if (state == InputState::Released) {
            std::cout << "Player stops moving" << std::endl;
        }
    }
}

void GameEventHandler::handleShooting(InputState state)
{
    if (_gameScene) {
        if (state == InputState::Pressed) {
            std::cout << "Player shoots!" << std::endl;
        }
    }
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