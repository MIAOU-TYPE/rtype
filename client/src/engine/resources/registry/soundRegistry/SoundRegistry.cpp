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
                                [](const auto &activeSound) {
                                    return !activeSound.sound->isPlaying();
                                }),
            _activeSounds.end());

        auto sound = _soundManager->createSound(handle, volume * (_globalSoundVolume / 100.f));
        if (!sound)
            return;

        sound->play();
        _activeSounds.push_back(ActiveSound{std::move(sound), volume});
    }

    void SoundRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;

        for (auto &activeSound : _activeSounds) {
            if (activeSound.sound && activeSound.sound->isPlaying()) {
                activeSound.sound->setVolume(activeSound.originalVolume * (volume / 100.f));
            }
        }
    }
} // namespace Engine
