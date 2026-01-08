/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** GameState
*/

#include "GameState.hpp"

namespace Engine
{
    GameState::GameState(std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry,
        std::shared_ptr<Graphics::IRenderer> renderer, World::ClientWorld &world)
        : _musicRegistry(std::move(musicRegistry)), _soundRegistry(std::move(soundRegistry)),
          _hud(std::make_unique<HUD>(renderer, world))
    {
    }

    void GameState::onEnter()
    {
        try {
            if (_musicRegistry && !_musicRegistry->isMusicPlaying()) {
                const float currentVolume = _musicRegistry->getMusicVolume();
                (void) _musicRegistry->loadAndPlayMusic("sounds/menu_theme.flac", true, currentVolume);
            }

            _hud->initialize();
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("{GameState::onEnter} ") + e.what());
        }
    }

    void GameState::update(StateManager &manager, const InputFrame &frame)
    {
        (void) manager;
        (void) frame;
    }

    void GameState::render()
    {
        _hud->render();
    }
} // namespace Engine
