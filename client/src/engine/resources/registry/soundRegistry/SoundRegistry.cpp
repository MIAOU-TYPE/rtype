/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SoundRegistry
*/

#include "SoundRegistry.hpp"

namespace Engine
{
    SoundRegistry::SoundRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager)
        : _soundManager(std::move(soundManager))
    {
        if (!_soundManager)
            throw SoundRegistryError("SoundRegistry: soundManager cannot be null");
    }

    void SoundRegistry::playSound(const AudioHandle handle, const float volume) const
    {
        if (!_soundManager->isValid(handle))
            return;

        _soundManager->setVolume(handle, volume * (_globalSoundVolume / 100.f));
        (void) _soundManager->play(handle);
    }

    void SoundRegistry::stopSound(const AudioHandle handle) const
    {
        if (!_soundManager->isValid(handle))
            return;
        
        _soundManager->stop(handle);
    }

    SoundRegistry::AudioHandle SoundRegistry::loadSound(const std::string &resourcePath)
    {
        if (!_soundManager)
            return Graphics::InvalidAudio;
        
        return _soundManager->load(resourcePath);
    }

    void SoundRegistry::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;
        _soundManager->setGlobalVolume(volume / 100.f);
    }

    float SoundRegistry::getSoundVolume() const noexcept
    {
        return _globalSoundVolume;
    }

    void SoundRegistry::setVolumeBeforeMute(const float volume)
    {
        _volumeBeforeMute = volume;
    }

    float SoundRegistry::getVolumeBeforeMute() const noexcept
    {
        return _volumeBeforeMute;
    }
} // namespace Engine
