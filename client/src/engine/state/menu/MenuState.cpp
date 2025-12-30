/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MenuState
*/

#include "MenuState.hpp"

namespace Engine
{
    MenuState::MenuState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
        std::shared_ptr<RoomService> roomService)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _roomService(std::move(roomService))
    {
    }

    void MenuState::onEnter(StateManager &)
    {
        try {
            _menu = std::make_unique<Menu>(_renderer);
            _menu->onEnter();
        } catch (const std::exception &e) {
            throw MenuError(std::string("{MenuState::onEnter} ") + e.what());
        }
    }

    void MenuState::update(StateManager &manager, const InputFrame &frame)
    {
        _menu->update(frame);

        if (_menu->wantsSettings()) {
            manager.queueState(std::make_unique<SettingsState>(_graphics, _renderer, _roomService));
            return;
        }
        if (_menu->wantsToStart()) {
            manager.queueState(std::make_unique<RoomState>(_graphics, _renderer, _roomService));
            return;
        }
        if (_menu->wantsToQuit())
            manager.queueQuit();
    }

    void MenuState::render()
    {
        _menu->render();
    }
} // namespace Engine
