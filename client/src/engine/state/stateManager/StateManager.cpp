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

    void StateManager::update(const float dt) const
    {
        if (_current)
            _current->update(dt);
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
} // namespace Engine
