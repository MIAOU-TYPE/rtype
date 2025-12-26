/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlAudioManager
*/

#include "SfmlAudioManager.hpp"
#include <iostream>

namespace Graphics
{
    SfmlAudioManager::SfmlAudioManager(std::shared_ptr<Resources::IResourceManager> resources)
        : _resources(std::move(resources))
    {
    }

    AudioHandle SfmlAudioManager::loadSound(const std::string &resourcePath)
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

    AudioHandle SfmlAudioManager::loadMusic(const std::string &resourcePath)
    {
        if (const auto it = _musicPathToHandle.find(resourcePath); it != _musicPathToHandle.end())
            return it->second;

        auto [data, size] = _resources->loadResource(resourcePath);
        if (!data || size == 0)
            return InvalidAudio;

        auto music = std::make_unique<sf::Music>();
        if (!music->openFromMemory(data, size))
            return InvalidAudio;

        AudioHandle handle = _nextHandle++;
        _musics.emplace(handle, MusicEntry{std::move(music), resourcePath});
        _musicPathToHandle.emplace(resourcePath, handle);

        return handle;
    }

    void SfmlAudioManager::playSound(AudioHandle handle, float volume)
    {
        const auto it = _sounds.find(handle);
        if (it == _sounds.end())
            return;

        _activeSounds.erase(
            std::remove_if(_activeSounds.begin(), _activeSounds.end(),
                [](const auto &sound) { return !sound->isPlaying(); }),
            _activeSounds.end());

        auto soundBuffer = std::make_shared<sf::SoundBuffer>(it->second.buffer);
        auto sound = std::make_unique<SfmlSound>(soundBuffer, volume * (_globalSoundVolume / 100.f));
        sound->play();
        _activeSounds.push_back(std::move(sound));
    }

    void SfmlAudioManager::playMusic(AudioHandle handle, bool loop, float volume)
    {
        const auto it = _musics.find(handle);
        if (it == _musics.end())
            return;

        if (_currentMusicHandle != InvalidAudio && _currentMusicHandle != handle)
            stopMusic();

        it->second.music->setLooping(loop);
        it->second.music->setVolume(volume);
        it->second.music->play();
        _currentMusicHandle = handle;
    }

    void SfmlAudioManager::stopMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        const auto it = _musics.find(_currentMusicHandle);
        if (it != _musics.end())
            it->second.music->stop();

        _currentMusicHandle = InvalidAudio;
    }

    void SfmlAudioManager::pauseMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        const auto it = _musics.find(_currentMusicHandle);
        if (it != _musics.end())
            it->second.music->pause();
    }

    void SfmlAudioManager::resumeMusic()
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        const auto it = _musics.find(_currentMusicHandle);
        if (it != _musics.end())
            it->second.music->play();
    }

    void SfmlAudioManager::setMusicVolume(float volume)
    {
        if (_currentMusicHandle == InvalidAudio)
            return;

        const auto it = _musics.find(_currentMusicHandle);
        if (it != _musics.end())
            it->second.music->setVolume(volume);
    }

    void SfmlAudioManager::setSoundVolume(float volume)
    {
        _globalSoundVolume = volume;
    }

    void SfmlAudioManager::unload(AudioHandle handle)
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

        if (auto it = _musics.find(handle); it != _musics.end()) {
            if (_currentMusicHandle == handle)
                stopMusic();
            for (auto pathIt = _musicPathToHandle.begin(); pathIt != _musicPathToHandle.end(); ++pathIt) {
                if (pathIt->second == handle) {
                    _musicPathToHandle.erase(pathIt);
                    break;
                }
            }
            _musics.erase(it);
        }
    }

    bool SfmlAudioManager::isValid(AudioHandle handle) const noexcept
    {
        return _sounds.contains(handle) || _musics.contains(handle);
    }

    void SfmlAudioManager::clear()
    {
        stopMusic();
        _sounds.clear();
        _musics.clear();
        _soundPathToHandle.clear();
        _musicPathToHandle.clear();
        _nextHandle = 1;
        _currentMusicHandle = InvalidAudio;
    }
} // namespace Graphics
