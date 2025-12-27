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
        _current = std::move(state);
        try {
            if (_current)
                _current->onEnter(*this);
            else
                _running = false;
        } catch (...) {
            _running = false;
        }
    }

    void StateManager::queueState(std::unique_ptr<IGameState> state)
    {
        _pending = std::move(state);
    }

    void StateManager::queueQuit()
    {
        _quitRequested = true;
    }

    void StateManager::update(const InputFrame &frame)
    {
        if (_current)
            _current->update(frame);

        if (_quitRequested) {
            _running = false;
            return;
        }

        if (_pending)
            changeState(std::move(_pending));
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
        _current.reset();
        _running = false;
    }
} // namespace Engine
