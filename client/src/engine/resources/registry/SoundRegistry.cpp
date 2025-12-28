/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SoundRegistry
*/

#include "SoundRegistry.hpp"
#include "SfmlSoundManager.hpp"

namespace Engine
{
    using Graphics::AudioHandle;
    using Graphics::InvalidAudio;

    SoundRegistry::SoundRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager)
        : _soundManager(std::move(soundManager))
    {
        _sfmlSoundManager = std::dynamic_pointer_cast<Graphics::SfmlSoundManager>(_soundManager);

        if (!_sfmlSoundManager)
            throw SoundRegistryError("SoundRegistry: soundManager must be a SfmlSoundManager");
    }

    void SoundRegistry::playSound(AudioHandle handle, float volume)
    {
        if (!_soundManager->isValid(handle))
            return;

        const sf::SoundBuffer *buffer = _sfmlSoundManager->getSoundBuffer(handle);
        if (!buffer)
            return;

        _activeSounds.erase(std::remove_if(_activeSounds.begin(), _activeSounds.end(),
                                [](const auto &sound) {
                                    return !sound->isPlaying();
                                }),
            _activeSounds.end());

        auto soundBuffer = std::shared_ptr<sf::SoundBuffer>(
            const_cast<sf::SoundBuffer *>(buffer),
            [](sf::SoundBuffer *) {}
        );
        auto sound = std::make_unique<Graphics::SfmlSound>(soundBuffer, volume * (_globalSoundVolume / 100.f));
        sound->play();
        _activeSounds.push_back(std::move(sound));
    }

    void SoundRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;
    }
} // namespace Engine
