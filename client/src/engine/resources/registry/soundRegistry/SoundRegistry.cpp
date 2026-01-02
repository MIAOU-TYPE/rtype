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

        _soundManager->setVolume(handle, volume * (_globalSoundVolume / 100.f));
        _soundManager->play(handle);
    }

    void SoundRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;
        _soundManager->setGlobalVolume(volume / 100.f);
    }
} // namespace Engine
