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
            _sounds[it->second].refCount++;
            return it->second;
        }

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidAudio;

        sf::SoundBuffer buffer;
        if (!buffer.loadFromMemory(data, size))
            return InvalidAudio;

        AudioHandle handle = _nextHandle++;
        _sounds.emplace(handle, SoundEntry{std::move(buffer), 1});
        _soundPathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlSoundManager::unload(const AudioHandle handle)
    {
        if (auto it = _sounds.find(handle); it != _sounds.end()) {
            if (--it->second.refCount == 0) {
                for (auto pathIt = _soundPathToHandle.begin(); pathIt != _soundPathToHandle.end(); ++pathIt) {
                    if (pathIt->second == handle) {
                        _soundPathToHandle.erase(pathIt);
                        break;
                    }
                }
                _sounds.erase(it);
            }
            return;
        }
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

        auto bufferPtr =
            std::shared_ptr<sf::SoundBuffer>(const_cast<sf::SoundBuffer *>(&it->second.buffer), [](sf::SoundBuffer *) {
            });
        return std::make_unique<SfmlSound>(bufferPtr, volume);
    }

    IAudioPlayable *SfmlSoundManager::get(AudioHandle) noexcept
    {
        return nullptr;
    }
} // namespace Graphics
