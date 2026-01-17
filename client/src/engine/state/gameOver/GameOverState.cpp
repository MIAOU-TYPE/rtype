/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameOverState
*/

#include "GameOverState.hpp"

namespace Engine
{
    GameOverState::GameOverState(std::shared_ptr<Graphics::IGraphics> graphics,
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<MusicRegistry> musicRegistry,
        std::shared_ptr<SoundRegistry> soundRegistry, std::shared_ptr<RoomManager> roomManager,
        std::shared_ptr<EventBus> eventBus, std::shared_ptr<AuthContext> authCtx,
        std::shared_ptr<ScoreboardContext> scoreCtx, std::weak_ptr<World::ClientWorld> world)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)), _eventBus(std::move(eventBus)),
          _authCtx(std::move(authCtx)), _scoreCtx(std::move(scoreCtx)), _world(std::move(world))
    {
    }

    void GameOverState::onEnter()
    {
        _menu = std::make_unique<GameOverMenu>(_renderer, _world);
        _menu->onEnter();
    }

    void GameOverState::update(StateManager &manager, const InputFrame &frame)
    {
        _menu->update(frame);
        if (_menu->wantsBackToMenu()) {
            _eventBus->emit<LeaveRoomRequested>(LeaveRoomRequested{});
            manager.queueState(std::make_unique<MenuState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
            return;
        }
        if (_menu->wantsQuit())
            manager.queueQuit();
    }

    void GameOverState::render()
    {
        _menu->render();
    }
}