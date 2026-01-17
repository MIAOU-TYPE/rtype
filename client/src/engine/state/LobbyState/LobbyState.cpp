/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"

#include "Lobby.hpp"

namespace Engine
{
    LobbyState::LobbyState(std::shared_ptr<Graphics::IGraphics> graphics, std::shared_ptr<Graphics::IRenderer> renderer,
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
        std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus,
        std::shared_ptr<AuthContext> authCtx, std::shared_ptr<ScoreboardContext> scoreCtx)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)),
          _eventBus(std::move(eventBus)), _authCtx(std::move(authCtx)), _scoreCtx(std::move(scoreCtx))
    {
    }

    void LobbyState::onEnter()
    {
        _lobbyPage = std::make_unique<Lobby>(_renderer);
        _lobbyPage->layout();
        _lobbyPage->onEnter();
    }

    void LobbyState::update(StateManager &manager, const InputFrame &frame)
    {
        (void) manager;

        if (_lobbyPage->wantsStart()) {
            _lobbyPage->consumeStart();
            _eventBus->emit<StartGameRequested>(StartGameRequested{});
            return;
        }
        if (_lobbyPage->wantsLeave()) {
            _lobbyPage->consumeLeave();
            _eventBus->emit<LeaveRoomRequested>(LeaveRoomRequested{});
            return;
        }
        if (_lobbyPage->needsUpdate()) {
            _lobbyPage->consumeUpdate();
            _eventBus->emit<UpdateRoomRequested>(UpdateRoomRequested{});
        }
        _lobbyPage->update(frame);
        _lobbyPage->setLobbyName(_roomManager->currentRoomData().roomName);
        _lobbyPage->setPlayers(_roomManager->currentRoomData().playerNames);
        _lobbyPage->setMaxPlayers(_roomManager->currentRoomData().maxPlayers);
    }

    void LobbyState::render()
    {
        _lobbyPage->render();
    }
} // namespace Engine
