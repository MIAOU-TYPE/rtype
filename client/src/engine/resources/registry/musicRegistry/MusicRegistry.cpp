/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MusicRegistry
*/

#include "MusicRegistry.hpp"

namespace Engine
{
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

        _globalMusicVolume = volume;
        _musicManager->setLooping(handle, loop);
        _musicManager->setVolume(handle, volume);
        (void) _musicManager->play(handle);
        _currentMusicHandle = handle;
    }

    void MusicRegistry::stopMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        _musicManager->stop(_currentMusicHandle);
        _currentMusicHandle = InvalidAudio;
    }

    bool MusicRegistry::isMusicPlaying() const noexcept
    {
        return _currentMusicHandle != InvalidAudio;
    }

    void MusicRegistry::setMusicVolume(float volume)
    {
        _globalMusicVolume = volume;
        if (_currentMusicHandle == InvalidAudio)
            return;

        _musicManager->setVolume(_currentMusicHandle, volume);
    }

    float MusicRegistry::getMusicVolume() const noexcept
    {
        return _globalMusicVolume;
    }

    void MusicRegistry::setVolumeBeforeMute(float volume)
    {
        _volumeBeforeMute = volume;
    }

    float MusicRegistry::getVolumeBeforeMute() const noexcept
    {
        return _volumeBeforeMute;
    }

    bool MusicRegistry::loadAndPlayMusic(const std::string &path, bool loop, float volume) noexcept
    {
        auto handle = _musicManager->load(path);
        if (handle == InvalidAudio)
            return false;

        playMusic(handle, loop, volume);
        return true;
    }
} // namespace Engine
