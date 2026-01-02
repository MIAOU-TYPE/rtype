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

        auto music = std::make_unique<SfmlMusic>(_resources, resourcePath);
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

    bool SfmlMusicManager::play(AudioHandle handle)
    {
        auto it = _musics.find(handle);
        if (it != _musics.end()) {
            it->second.music->play();
            return true;
        }
        return false;
    }

    void SfmlMusicManager::stop(AudioHandle handle)
    {
        auto it = _musics.find(handle);
        if (it != _musics.end()) {
            it->second.music->stop();
        }
    }

    void SfmlMusicManager::setVolume(AudioHandle handle, float volume)
    {
        auto it = _musics.find(handle);
        if (it != _musics.end()) {
            it->second.music->setVolume(volume);
        }
    }

    void SfmlMusicManager::setLooping(AudioHandle handle, bool loop)
    {
        auto it = _musics.find(handle);
        if (it != _musics.end()) {
            it->second.music->setLooping(loop);
        }
    }

    void SfmlMusicManager::setGlobalVolume(float multiplier)
    {
        for (auto &[handle, entry] : _musics) {
            if (entry.music->isPlaying()) {
                entry.music->setVolume(entry.music->getVolume() * multiplier);
            }
        }
    }
} // namespace Graphics