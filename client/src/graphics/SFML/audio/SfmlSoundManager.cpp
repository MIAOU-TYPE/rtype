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

        auto buffer = std::make_shared<sf::SoundBuffer>();
        if (!buffer->loadFromMemory(data, size))
            return InvalidAudio;

        AudioHandle handle = _nextHandle++;
        _sounds.emplace(handle, SoundEntry{buffer, resourcePath});
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
        _nextHandle = 1;
    }

    std::unique_ptr<IAudioPlayable> SfmlSoundManager::createSound(const AudioHandle handle, float volume) const noexcept
    {
        auto it = _sounds.find(handle);
        if (it == _sounds.end())
            return nullptr;

        return std::make_unique<SfmlSound>(it->second.buffer, volume);
    }

    IAudioPlayable *SfmlSoundManager::get(AudioHandle) noexcept
    {
        return nullptr;
    }
} // namespace Graphics
