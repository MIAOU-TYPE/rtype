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
        if (_current) {
            _current->onEnter(*this);
            _current->update(0.f);
        } else
            _running = false;
    }

    void StateManager::update(const float dt) const
    {
        if (_current)
            _current->update(dt);
        if (_overlay)
            _overlay->update(dt);
    }

    void StateManager::render() const
    {
        if (_current)
            _current->render();
        if (_overlay)
            _overlay->render();
    }

    bool StateManager::isRunning() const
    {
        return _running;
    }

    IGameState *StateManager::current() const noexcept
    {
        return _current.get();
    }

    void StateManager::pushOverlay(std::unique_ptr<IGameState> overlay)
    {
        _overlay = std::move(overlay);
        _overlay->onEnter(*this);
    }

    void StateManager::popOverlay()
    {
        if (_overlay)
            _overlay->onExit();
        _overlay.reset();
    }

    bool StateManager::hasOverlay() const noexcept
    {
        return static_cast<bool>(_overlay);
    }
} // namespace Engine
