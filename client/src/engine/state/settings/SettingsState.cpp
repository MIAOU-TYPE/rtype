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
        std::shared_ptr<Graphics::IRenderer> renderer, std::shared_ptr<MusicRegistry> musicRegistry,
        std::shared_ptr<SoundRegistry> soundRegistry, std::shared_ptr<RoomManager> roomManager,
        std::shared_ptr<EventBus> eventBus, std::shared_ptr<AuthContext> authCtx,
        std::shared_ptr<ScoreboardContext> scoreCtx)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)),
          _eventBus(std::move(eventBus)), _authCtx(std::move(authCtx)), _scoreCtx(std::move(scoreCtx))
    {
    }

    void SettingsState::onEnter()
    {
        _menu = std::make_unique<SettingsMenu>(_renderer, _musicRegistry, _soundRegistry);
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
        if (_menu->wantsBack()) {
            (void) Utils::SettingsConfig::getInstance().saveIfDirty();
            manager.queueState(std::make_unique<MenuState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
            }
    }

    void SettingsState::render()
    {
        _menu->render();
    }
} // namespace Engine
