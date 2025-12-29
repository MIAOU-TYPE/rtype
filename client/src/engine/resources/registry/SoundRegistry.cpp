/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SoundRegistry
*/

#include "SoundRegistry.hpp"
#include <algorithm>

namespace Engine
{
    SoundRegistry::SoundRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager)
        : _soundManager(std::move(soundManager))
    {
        if (!_soundManager)
            throw SoundRegistryError("SoundRegistry: soundManager cannot be null");
    }

    void SoundRegistry::playSound(AudioHandle handle, float volume)
    {
        if (!_soundManager->isValid(handle))
            return;

        _activeSounds.erase(std::remove_if(_activeSounds.begin(), _activeSounds.end(),
                                [](const auto &sound) {
                                    return !sound->isPlaying();
                                }),
            _activeSounds.end());

        auto sound = _soundManager->createSound(handle, volume * (_globalSoundVolume / 100.f));
        if (!sound)
            return;

        sound->play();
        _activeSounds.push_back(std::move(sound));
    }

    void SoundRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;

        for (auto &sound : _activeSounds) {
            if (sound && sound->isPlaying()) {
                sound->setVolume(volume);
            }
        }
    }
} // namespace Engine
