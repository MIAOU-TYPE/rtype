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
        std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus,
        std::shared_ptr<AuthContext> authCtx)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)),
          _eventBus(std::move(eventBus)), _authCtx(std::move(authCtx))
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
        if (_menu->wantsBackToMenu()) {
            manager.queueState(std::make_unique<MenuState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx));
            return;
        }
        if (_menu->wantsCreateRoom()) {
            _menu->consumeCreateRoomState();
            const std::string levelPath = _menu->levelSelected();
            _eventBus->emit<CreateRoomRequested>(
                CreateRoomRequested("default", _menu->maxPlayerSelected(), _menu->difficultySelected(),
                    levelPath.empty() ? "levels/space_level1.json" : levelPath));
        }
        if (_menu->wantsListRooms()) {
            _menu->consumeListRoomsRequest();
            _eventBus->emit<ListRoomRequested>(ListRoomRequested());
        }
        if (_menu->wantsJoinRoom()) {
            _menu->consumeJoinRoomState();
            const auto id = _menu->roomIdSelected();
            _eventBus->emit<JoinRoomRequested>(JoinRoomRequested(id));
        }
    }

    void RoomState::render()
    {
        _menu->render();
    }
} // namespace Engine
