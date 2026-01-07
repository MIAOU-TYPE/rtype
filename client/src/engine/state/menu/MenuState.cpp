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
        std::shared_ptr<RoomManager> roomManager, std::shared_ptr<EventBus> eventBus)
        : _graphics(std::move(graphics)), _renderer(std::move(renderer)), _musicRegistry(std::move(musicRegistry)),
          _soundRegistry(std::move(soundRegistry)), _roomManager(std::move(roomManager)), _eventBus(std::move(eventBus))
    {
    }

    void MenuState::onEnter()
    {
        try {
            _menu = std::make_unique<Menu>(_renderer);
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
        _menu->update(frame);

        if (_menu->wantsSettings()) {
            manager.queueState(std::make_unique<SettingsState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus));
            return;
        }
        if (_menu->wantsToStart()) {
            manager.queueState(std::make_unique<RoomState>(
                _graphics, _renderer, _musicRegistry, _soundRegistry, _roomManager, _eventBus));
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
