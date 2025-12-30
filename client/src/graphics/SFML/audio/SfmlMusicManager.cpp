/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusicManager
*/

#include "SfmlMusicManager.hpp"

namespace Graphics
{
    SfmlMusicManager::SfmlMusicManager(std::shared_ptr<Resources::IResourceManager> resources)
        : _resources(std::move(resources))
    {
    }

    AudioHandle SfmlMusicManager::load(const std::string &resourcePath)
    {
        if (const auto it = _musicPathToHandle.find(resourcePath); it != _musicPathToHandle.end())
            return it->second;

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidAudio;

        auto sfmlMusic = std::make_unique<sf::Music>();
        if (!sfmlMusic->openFromMemory(data, size))
            return InvalidAudio;

        auto music = std::make_unique<SfmlMusic>(std::move(sfmlMusic));
        AudioHandle handle = _nextHandle++;
        _musics.emplace(handle, MusicEntry{std::move(music), resourcePath});
        _musicPathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlMusicManager::unload(const AudioHandle handle)
    {
        auto it = _musics.find(handle);
        if (it == _musics.end())
            return;

        if (it->second.music && it->second.music->isPlaying()) {
            it->second.music->stop();
        }
        _musicPathToHandle.erase(it->second.resourcePath);
        _musics.erase(it);
    }

    bool SfmlMusicManager::isValid(const AudioHandle handle) const noexcept
    {
        return _musics.contains(handle);
    }

    void SfmlMusicManager::clear()
    {
        for (auto &[handle, entry] : _musics) {
            if (entry.music && entry.music->isPlaying()) {
                entry.music->stop();
            }
        }
        _musics.clear();
        _musicPathToHandle.clear();
        _nextHandle = 1;
    }

    std::unique_ptr<IAudioPlayable> SfmlMusicManager::createSound(AudioHandle, float) const noexcept
    {
        return nullptr;
    }

    std::shared_ptr<IAudioPlayable> SfmlMusicManager::get(const AudioHandle handle) noexcept
    {
        if (auto it = _musics.find(handle); it != _musics.end()) {
            return std::shared_ptr<IAudioPlayable>(it->second.music.get(), [](IAudioPlayable *) {});
        }
        return nullptr;
    }
} // namespace Graphics
