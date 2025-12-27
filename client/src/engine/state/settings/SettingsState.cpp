/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace Engine
{
    SettingsState::SettingsState(
        std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer))
    {
    }

    void SettingsState::onEnter(StateManager &manager)
    {
        _manager = &manager;
        _menu = std::make_unique<SettingsMenu>(_renderer);
        _menu->onEnter();
    }

    void SettingsState::update(const InputFrame &frame)
    {
        if (_pendingResize) {
            _menu->layout();
            _pendingResize = false;
        }
        _menu->update(frame);
        if (_menu->resolutionChanged()) {
            _graphics->setResolution(_menu->currentResolution());
            _pendingResize = true;
        }
        if (_menu->wantsBack())
            _manager->queueState(std::make_unique<MenuState>(_graphics, _renderer));
    }

    void SettingsState::render()
    {
        _menu->render();
    }
} // namespace Engine
