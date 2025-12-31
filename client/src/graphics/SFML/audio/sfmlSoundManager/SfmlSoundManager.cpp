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

        AudioHandle handle = _nextHandle++;
        _sounds.emplace(handle, SoundEntry{resourcePath});
        _soundPathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlSoundManager::unload(const AudioHandle handle)
    {
        auto it = _sounds.find(handle);
        if (it == _sounds.end())
            return;

        _soundPathToHandle.erase(it->second.resourcePath);
        _sounds.erase(it);
    }

    bool SfmlSoundManager::isValid(const AudioHandle handle) const noexcept
    {
        return _sounds.contains(handle);
    }

    void SfmlSoundManager::clear()
    {
        _sounds.clear();
        _soundPathToHandle.clear();
        _activeSounds.clear();
        _nextHandle = 1;
    }

    void SfmlSoundManager::play(AudioHandle handle)
    {
        auto it = _sounds.find(handle);
        if (it == _sounds.end())
            return;

        try {
            auto sound = std::make_unique<SfmlSound>(_resources, it->second.resourcePath);
            sound->play();
            _activeSounds.push_back({handle, std::move(sound)});
        } catch (const AudioError &) {
            // Ignore
        }
    }

    void SfmlSoundManager::stop(AudioHandle handle)
    {
        for (auto it = _activeSounds.begin(); it != _activeSounds.end();) {
            if (it->handle == handle) {
                it->sound->stop();
                it = _activeSounds.erase(it);
            } else {
                ++it;
            }
        }
    }

    void SfmlSoundManager::setVolume(AudioHandle handle, float volume)
    {
        for (auto &active : _activeSounds) {
            if (active.handle == handle) {
                active.sound->setVolume(volume);
            }
        }
    }

    void SfmlSoundManager::setLooping(AudioHandle handle, bool loop)
    {
        for (auto &active : _activeSounds) {
            if (active.handle == handle) {
                active.sound->setLooping(loop);
            }
        }
    }

    void SfmlSoundManager::setGlobalVolume(float multiplier)
    {
        for (auto &active : _activeSounds) {
            active.sound->setVolume(active.sound->getVolume() * multiplier);
        }
    }
} // namespace Graphics
