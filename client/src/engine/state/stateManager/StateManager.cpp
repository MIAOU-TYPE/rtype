/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** StateManager
*/

#include "StateManager.hpp"

namespace Engine
{
    void StateManager::changeState(std::unique_ptr<IGameState> state)
    {
        if (_current)
            _current->onExit();
        _current = std::move(state);
        if (_current)
            _current->onEnter(*this);
        else
            _running = false;
    }

    void StateManager::update() const
    {
        if (_current)
            _current->update();
    }

    void StateManager::render() const
    {
        if (_current)
            _current->render();
    }

    bool StateManager::isRunning() const
    {
        return _running;
    }

    void StateManager::requestQuit()
    {
        if (_current)
            _current->onExit();
        _current.reset();
        _running = false;
    }
} // namespace Engine
