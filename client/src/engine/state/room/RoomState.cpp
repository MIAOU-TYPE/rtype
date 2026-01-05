/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RoomState
*/

#include "RoomState.hpp"

namespace Engine
{
    RoomState::RoomState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
        std::shared_ptr<RoomManager> roomManager)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _roomManager(std::move(roomManager))
    {
    }

    void RoomState::onEnter()
    {
        _menu = std::make_unique<RoomMenu>(_renderer, _roomManager);
        _menu->layout();
    }

    void RoomState::update(StateManager &manager, const InputFrame &frame)
    {
        _menu->update(frame);
        if (_menu->wantsBackToMenu())
            manager.queueState(std::make_unique<MenuState>(_graphics, _renderer, _roomManager));
    }

    void RoomState::render()
    {
        _menu->render();
    }
} // namespace Engine
