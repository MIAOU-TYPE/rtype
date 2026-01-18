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
        const std::shared_ptr<Graphics::IRenderer> &renderer, std::function<int()> getScore,
        std::function<int()> getCurrentLife, std::function<int()> getMaxLife, std::shared_ptr<RoomManager> roomManager)
        : _musicRegistry(std::move(musicRegistry)), _soundRegistry(std::move(soundRegistry)),
          _hud(std::make_unique<HUD>(renderer, getScore, getCurrentLife, getMaxLife)),
          _roomManager(std::move(roomManager))
    {
    }

    void GameState::onEnter()
    {
        try {
            if (_musicRegistry) {
                std::string musicPath = ::DEFAULT_GAME_MUSIC;
                if (_roomManager) {
                    const auto &roomData = _roomManager->currentRoomData();
                    if (!roomData.gameConfig.worldMusic.empty())
                        musicPath = roomData.gameConfig.worldMusic;
                }
                _musicRegistry->stopMusic();
                const float currentVolume = _musicRegistry->getMusicVolume();
                (void) _musicRegistry->loadAndPlayMusic(musicPath, true, currentVolume);
            }
        } catch (const std::exception &e) {
            throw GameStateError(std::string("{GameState::onEnter} ") + e.what());
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
