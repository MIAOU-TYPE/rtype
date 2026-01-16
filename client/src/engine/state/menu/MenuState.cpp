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
        std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
        std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus,
        std::shared_ptr<AuthContext> authCtx, std::shared_ptr<ScoreboardContext> scoreCtx)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)),
          _eventBus(std::move(eventBus)), _authCtx(std::move(authCtx)), _scoreCtx(std::move(scoreCtx))
    {
    }

    void MenuState::onEnter()
    {
        try {
            _menu = std::make_unique<Menu>(_renderer);
            _lastScoreVersion = _scoreCtx ? _scoreCtx->version.load(std::memory_order_acquire) : 0;
            if (_authCtx) {
                {
                    std::scoped_lock lk(_authCtx->m);
                    _authCtx->authError.clear();
                }
                _lastAuthErrorVersion = _authCtx->authErrorVersion.fetch_add(1, std::memory_order_release) + 1;
            }
            const bool authedNow = _authCtx && _authCtx->authed.load(std::memory_order_acquire);
            _lastAuthed = authedNow;
            _menu->setAuthed(authedNow);
            _menu->onEnter();

            if (_musicRegistry && !_musicRegistry->isMusicPlaying()) {
                const float currentVolume = _musicRegistry->getMusicVolume();
                (void) _musicRegistry->loadAndPlayMusic("sounds/menu_theme.flac", true, currentVolume);
            }
        } catch (const std::exception &e) {
            throw MenuError(std::string("{MenuState::onEnter} ") + e.what());
        }
    }

    void MenuState::update(StateManager &manager, const InputFrame &frame)
    {
        if (const bool authedNow = _authCtx && _authCtx->authed.load(std::memory_order_acquire);
            authedNow != _lastAuthed) {
            _menu->setAuthed(authedNow);
            if (authedNow)
                _menu->onEnter();
            _lastAuthed = authedNow;
        }

        _menu->update(frame);

        if (_menu->wantsScoreboardRefresh()) {
            _eventBus->emit<ScoreboardGetRequested>(ScoreboardGetRequested(10));
            _menu->consumeScoreboardRefresh();
        }
        if (_scoreCtx) {
            if (const uint32_t ver = _scoreCtx->version.load(std::memory_order_acquire); ver != _lastScoreVersion) {
                std::vector<ScoreEntry> copy;
                {
                    std::scoped_lock lk(_scoreCtx->m);
                    copy = _scoreCtx->scores;
                }
                _menu->setScoreboard(std::move(copy));
                _lastScoreVersion = ver;
            }
        }
        if (_authCtx) {
            if (const uint32_t ver = _authCtx->authErrorVersion.load(std::memory_order_acquire);
                ver != _lastAuthErrorVersion) {
                std::string msg;
                {
                    std::scoped_lock lk(_authCtx->m);
                    msg = _authCtx->authError;
                }
                if (msg.empty())
                    _menu->clearAuthError();
                else
                    _menu->setAuthError(std::move(msg));
                _lastAuthErrorVersion = ver;
            }
        }
        if (_menu->wantsSettings()) {
            manager.queueState(std::make_unique<SettingsState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
            return;
        }
        if (_menu->wantsLevelEditor()) {
            manager.queueState(std::make_unique<LevelEditorState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
            return;
        }
        if (_menu->isAuthed() && _menu->wantsToStart()) {
            manager.queueState(std::make_unique<RoomState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus, _authCtx, _scoreCtx));
            return;
        }
        if (_menu->hasAuthSubmission()) {
            const auto mode = _menu->submittedMode();
            const auto &u = _menu->submittedUsername();
            const auto &p = _menu->submittedPassword();
            _menu->clearAuthError();
            if (_authCtx) {
                {
                    std::scoped_lock lk(_authCtx->m);
                    _authCtx->authError.clear();
                }
                _authCtx->authErrorVersion.fetch_add(1, std::memory_order_release);
            }
            if (mode == Menu::AuthMode::Login)
                _eventBus->emit<AuthLoginRequested>(AuthLoginRequested(u, p));
            else if (mode == Menu::AuthMode::Register)
                _eventBus->emit<AuthRegisterRequested>(AuthRegisterRequested(u, p));
            _menu->consumeAuthSubmission();
        }
        if (_menu->wantsToQuit())
            manager.queueQuit();
    }

    void MenuState::render()
    {
        _menu->render();
    }
} // namespace Engine
