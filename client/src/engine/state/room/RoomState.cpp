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
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
        std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)), _eventBus(std::move(eventBus))
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
            manager.queueState(std::make_unique<MenuState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus));
        if (auto &create = _menu->wantsCreateRoom()) {
            create = false;
            _eventBus->emit<CreateRoomRequested>(CreateRoomRequested("default", 2));
        }
        if (auto &join = _menu->wantsJoinRoom()) {
            join = false;
            _eventBus->emit<JoinRoomRequested>(JoinRoomRequested(1));
        }
    }

    void RoomState::render()
    {
        _menu->render();
    }
} // namespace Engine
