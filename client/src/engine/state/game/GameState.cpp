/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** GameState
*/

#include "GameState.hpp"

namespace Engine
{
    GameState::GameState(std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry)
        : _musicRegistry(std::move(musicRegistry)), _soundRegistry(std::move(soundRegistry))
    {
    }

    void GameState::onEnter()
    {
        try {
            if (_musicRegistry && !_musicRegistry->isMusicPlaying()) {
                const float currentVolume = _musicRegistry->getMusicVolume();
                (void) _musicRegistry->loadAndPlayMusic("sounds/menu_theme.flac", true, currentVolume);
            }
        } catch (const std::exception &e) {
            throw MenuError(std::string("{GameState::onEnter} ") + e.what());
        }
    }

    void GameState::update(StateManager &manager, const InputFrame &frame)
    {
        (void) manager;
        (void) frame;
    }

    void GameState::render()
    {
    }
} // namespace Engine
