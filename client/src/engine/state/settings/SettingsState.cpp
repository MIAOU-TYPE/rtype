/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace Engine
{
    SettingsState::SettingsState(std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<RoomManager> roomManager)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _roomManager(std::move(roomManager))
    {
    }

    void SettingsState::onEnter(StateManager &manager)
    {
        (void) manager;
        _menu = std::make_unique<SettingsMenu>(_renderer);
        _menu->onEnter();
    }

    void SettingsState::update(StateManager &manager, const InputFrame &frame)
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
            manager.queueState(std::make_unique<MenuState>(_graphics, _renderer, _roomManager));
    }

    void SettingsState::render()
    {
        _menu->render();
    }
} // namespace Engine
