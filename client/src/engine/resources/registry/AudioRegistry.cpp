/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AudioRegistry
*/

#include "AudioRegistry.hpp"
#include "SfmlSoundManager.hpp"
#include "SfmlMusicManager.hpp"

namespace Engine
{
    using Graphics::AudioHandle;
    using Graphics::InvalidAudio;

    AudioRegistry::AudioRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager,
        std::shared_ptr<Graphics::IAudioManager> musicManager)
        : _soundManager(std::move(soundManager)), _musicManager(std::move(musicManager))
    {
        _sfmlSoundManager = std::dynamic_pointer_cast<Graphics::SfmlSoundManager>(_soundManager);
        _sfmlMusicManager = std::dynamic_pointer_cast<Graphics::SfmlMusicManager>(_musicManager);

        if (!_sfmlSoundManager)
            throw AudioRegistryError("AudioRegistry: soundManager must be a SfmlSoundManager");
        if (!_sfmlMusicManager)
            throw AudioRegistryError("AudioRegistry: musicManager must be a SfmlMusicManager");
    }

    void AudioRegistry::playSound(AudioHandle handle, float volume)
    {
        if (!_soundManager->isValid(handle))
            return;

        const sf::SoundBuffer* buffer = _sfmlSoundManager->getSoundBuffer(handle);
        if (!buffer)
            return;

        _activeSounds.erase(
            std::remove_if(_activeSounds.begin(), _activeSounds.end(),
                [](const auto &sound) { return !sound->isPlaying(); }),
            _activeSounds.end());

        auto soundBuffer = std::make_shared<sf::SoundBuffer>(*buffer);
        auto sound = std::make_unique<Graphics::SfmlSound>(soundBuffer, volume * (_globalSoundVolume / 100.f));
        sound->play();
        _activeSounds.push_back(std::move(sound));
    }

    void AudioRegistry::playMusic(AudioHandle handle, bool loop, float volume)
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

    void AudioRegistry::stopMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        if (Graphics::SfmlMusic* music = _sfmlMusicManager->getMusic(_currentMusicHandle)) {
            music->stop();
        }
        _currentMusicHandle = InvalidAudio;
    }

    void AudioRegistry::setMusicVolume(float volume)
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        if (Graphics::SfmlMusic* music = _sfmlMusicManager->getMusic(_currentMusicHandle)) {
            music->setVolume(volume);
        }
    }

    void AudioRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;
    }
} // namespace Engine