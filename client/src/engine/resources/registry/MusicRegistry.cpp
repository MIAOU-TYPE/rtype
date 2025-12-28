/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MusicRegistry
*/

#include "MusicRegistry.hpp"
#include "SfmlMusicManager.hpp"
#include "SfmlMusic.hpp"

namespace Engine
{
    using Graphics::AudioHandle;
    using Graphics::InvalidAudio;

    MusicRegistry::MusicRegistry(std::shared_ptr<Graphics::IAudioManager> musicManager)
        : _musicManager(std::move(musicManager))
    {
        _sfmlMusicManager = std::dynamic_pointer_cast<Graphics::SfmlMusicManager>(_musicManager);

        if (!_sfmlMusicManager)
            throw MusicRegistryError("MusicRegistry: musicManager must be a SfmlMusicManager");
    }

    void MusicRegistry::playMusic(AudioHandle handle, bool loop, float volume)
    {
        if (!_musicManager->isValid(handle))
            return;

        if (Graphics::SfmlMusic* music = _sfmlMusicManager->getMusic(handle)) {
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

        if (Graphics::SfmlMusic* music = _sfmlMusicManager->getMusic(_currentMusicHandle)) {
            music->stop();
        }
        _currentMusicHandle = InvalidAudio;
    }

    void MusicRegistry::setMusicVolume(float volume)
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        if (Graphics::SfmlMusic* music = _sfmlMusicManager->getMusic(_currentMusicHandle)) {
            music->setVolume(volume);
        }
    }
} // namespace Engine
