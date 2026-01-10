/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSoundManager
*/

#include "SfmlSoundManager.hpp"

namespace Graphics
{
    SfmlSoundManager::SfmlSoundManager(std::shared_ptr<Resources::IResourceManager> resources)
        : _resources(std::move(resources))
    {
    }

    AudioHandle SfmlSoundManager::load(const std::string &resourcePath)
    {
        if (const auto it = _soundPathToHandle.find(resourcePath); it != _soundPathToHandle.end()) {
            return it->second;
        }

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidAudio;

        AudioHandle audioHandle = _nextHandle++;
        _sounds.emplace(audioHandle, SoundEntry{resourcePath});
        _soundPathToHandle.emplace(resourcePath, audioHandle);

        return audioHandle;
    }

    void SfmlSoundManager::unload(const AudioHandle audioHandle)
    {
        const auto it = _sounds.find(audioHandle);
        if (it == _sounds.end())
            return;

        _soundPathToHandle.erase(it->second.resourcePath);
        _sounds.erase(it);
    }

    bool SfmlSoundManager::isValid(const AudioHandle audioHandle) const noexcept
    {
        return _sounds.contains(audioHandle);
    }

    void SfmlSoundManager::clear()
    {
        _sounds.clear();
        _soundPathToHandle.clear();
        _activeSounds.clear();
        _nextHandle = 1;
    }

    bool SfmlSoundManager::play(const AudioHandle audioHandle)
    {
        const auto it = _sounds.find(audioHandle);
        if (it == _sounds.end())
            return false;

        try {
            auto sound = std::make_unique<SfmlSound>(_resources, it->second.resourcePath);
            sound->play();
            _activeSounds.push_back({audioHandle, std::move(sound)});
            return true;
        } catch (...) {
            return false;
        }
    }

    void SfmlSoundManager::stop(const AudioHandle audioHandle)
    {
        for (auto it = _activeSounds.begin(); it != _activeSounds.end();) {
            if (it->handle == audioHandle) {
                it->sound->stop();
                it = _activeSounds.erase(it);
            } else {
                ++it;
            }
        }
    }

    void SfmlSoundManager::setVolume(const AudioHandle audioHandle, const float volume)
    {
        for (auto &[handle, sound] : _activeSounds) {
            if (handle == audioHandle) {
                sound->setVolume(volume);
            }
        }
    }

    void SfmlSoundManager::setLooping(const AudioHandle audioHandle, const bool loop)
    {
        for (auto &[handle, sound] : _activeSounds) {
            if (handle == audioHandle) {
                sound->setLooping(loop);
            }
        }
    }

    void SfmlSoundManager::setGlobalVolume(const float multiplier)
    {
        for (auto &[audioHandle, sound] : _activeSounds) {
            sound->setVolume(sound->getVolume() * multiplier);
        }
    }
} // namespace Graphics
