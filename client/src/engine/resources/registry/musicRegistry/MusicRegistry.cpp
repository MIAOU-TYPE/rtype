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

        if (_currentMusicHandle != InvalidAudio && _currentMusicHandle != handle)
            stopMusic();

        _musicManager->setLooping(handle, loop);
        _musicManager->setVolume(handle, volume);
        _musicManager->play(handle);
        _currentMusicHandle = handle;
    }

    void MusicRegistry::stopMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        _musicManager->stop(_currentMusicHandle);
        _currentMusicHandle = InvalidAudio;
    }

    void MusicRegistry::setMusicVolume(float volume)
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        _musicManager->setVolume(_currentMusicHandle, volume);
    }
} // namespace Engine
