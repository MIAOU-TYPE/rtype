/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MusicRegistry
*/

#include "MusicRegistry.hpp"

namespace Engine
{
    using Graphics::InvalidAudio;

    MusicRegistry::MusicRegistry(std::shared_ptr<Graphics::IAudioManager> musicManager)
        : _musicManager(std::move(musicManager))
    {
        if (!_musicManager)
            throw MusicRegistryError("MusicRegistry: musicManager cannot be null");
    }

    void MusicRegistry::playMusic(AudioHandle handle, bool loop, float volume)
    {
        if (!_musicManager->isValid(handle))
            return;

        if (Graphics::IAudioPlayable *music = _musicManager->get(handle)) {
            if (_currentMusicHandle != InvalidAudio && _currentMusicHandle != handle)
                stopMusic();

            music->setLooping(loop);
            music->setVolume(volume);
            music->play();
            _currentMusicHandle = handle;
        }
    }

    void MusicRegistry::stopMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        if (Graphics::IAudioPlayable *music = _musicManager->get(_currentMusicHandle)) {
            music->stop();
        }
        _currentMusicHandle = InvalidAudio;
    }

    void MusicRegistry::setMusicVolume(float volume)
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        if (Graphics::IAudioPlayable *music = _musicManager->get(_currentMusicHandle)) {
            music->setVolume(volume);
        }
    }
} // namespace Engine
