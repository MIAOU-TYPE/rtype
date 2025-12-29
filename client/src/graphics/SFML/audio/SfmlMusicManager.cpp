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

    AudioHandle SfmlMusicManager::loadAudio(const std::string &resourcePath)
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

    void SfmlMusicManager::unload(AudioHandle handle)
    {
        if (auto it = _musics.find(handle); it != _musics.end()) {
            if (it->second.music && it->second.music->isPlaying()) {
                it->second.music->stop();
            }
            for (auto pathIt = _musicPathToHandle.begin(); pathIt != _musicPathToHandle.end(); ++pathIt) {
                if (pathIt->second == handle) {
                    _musicPathToHandle.erase(pathIt);
                    break;
                }
            }
            _musics.erase(it);
        }
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

    SfmlMusic *SfmlMusicManager::getMusic(const AudioHandle handle) noexcept
    {
        if (auto it = _musics.find(handle); it != _musics.end()) {
            return it->second.music.get();
        }
        return nullptr;
    }
} // namespace Graphics
