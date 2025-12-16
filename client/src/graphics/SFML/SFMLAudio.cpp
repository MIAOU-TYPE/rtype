/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAudio
*/

#include "SFMLAudio.hpp"
#include <iostream>

using namespace Audio;

SFMLAudio::SFMLAudio(std::shared_ptr<Resources::EmbeddedResourceManager> resourceManager)
    : _resourceManager(std::move(resourceManager))
{
    if (!_resourceManager) {
        throw SFMLAudioError("ResourceManager cannot be null");
    }
    registerAudioResources();
}

void SFMLAudio::registerAudioResources()
{
    _audioInfo["shoot"] = {"sounds/shoot.wav", AudioType::Sound, 70.0f, false};
    _audioInfo["explosion"] = {"sounds/explosion.wav", AudioType::Sound, 80.0f, false};
    _audioInfo["hit"] = {"sounds/hit.wav", AudioType::Sound, 60.0f, false};
    _audioInfo["powerup"] = {"sounds/powerup.wav", AudioType::Sound, 75.0f, false};

    _audioInfo["menu_theme"] = {"sounds/menu_theme.flac", AudioType::Music, 50.0f, true};
    _audioInfo["game_theme"] = {"sounds/game_theme.flac", AudioType::Music, 50.0f, true};
    _audioInfo["boss_theme"] = {"sounds/boss_theme.flac", AudioType::Music, 60.0f, true};
}

void SFMLAudio::playSound(const std::string &soundName)
{
    auto it = _audioInfo.find(soundName);
    if (it == _audioInfo.end()) {
        throw SFMLAudioError("Sound '" + soundName + "' not found in audio registry");
    }

    if (it->second.type != AudioType::Sound) {
        throw SFMLAudioError("'" + soundName + "' is not a sound, use playMusic() instead");
    }

    if (_soundBuffers.find(soundName) == _soundBuffers.end()) {
        if (!loadSoundBuffer(soundName)) {
            throw SFMLAudioError("Failed to load sound buffer for '" + soundName + "'");
        }
    }

    auto sound = std::make_unique<sf::Sound>(_soundBuffers[soundName]);
    sound->setVolume(it->second.defaultVolume * (_globalSoundVolume / 100.0f));
    sound->setLooping(it->second.loop);
    sound->play();

    _activeSounds[soundName] = std::move(sound);
}

void SFMLAudio::playMusic(const std::string &musicName, bool loop)
{
    auto it = _audioInfo.find(musicName);
    if (it == _audioInfo.end()) {
        throw SFMLAudioError("Music '" + musicName + "' not found in audio registry");
    }

    if (it->second.type != AudioType::Music) {
        throw SFMLAudioError("'" + musicName + "' is not music, use playSound() instead");
    }

    if (_music && _music->getStatus() == sf::SoundSource::Status::Playing) {
        _music->stop();
    }

    auto resourceData = _resourceManager->loadResource(it->second.path);
    if (resourceData.data == nullptr || resourceData.size == 0) {
        throw SFMLAudioError("Failed to load embedded music resource '" + it->second.path + "'");
    }

    try {
        _music = std::make_unique<sf::Music>();
        if (!_music->openFromMemory(resourceData.data, resourceData.size)) {
            throw SFMLAudioError("Failed to open music from memory for '" + it->second.path + "'");
        }
    } catch (const std::exception &e) {
        throw SFMLAudioError("Failed to load music from '" + it->second.path + "': " + std::string(e.what()));
    }

    _music->setVolume(it->second.defaultVolume * (_musicVolume / 100.0f));
    _music->setLooping(loop);
    _music->play();
}

void SFMLAudio::stopMusic()
{
    if (_music) {
        _music->stop();
    }
}

void SFMLAudio::pauseMusic()
{
    if (_music && _music->getStatus() == sf::SoundSource::Status::Playing) {
        _music->pause();
    }
}

void SFMLAudio::resumeMusic()
{
    if (_music && _music->getStatus() == sf::SoundSource::Status::Paused) {
        _music->play();
    }
}

void SFMLAudio::setSoundVolume(const std::string &soundName, float volume)
{
    if (volume < 0.0f || volume > 100.0f) {
        throw SFMLAudioError("Volume must be between 0.0 and 100.0");
    }

    auto infoIt = _audioInfo.find(soundName);
    if (infoIt == _audioInfo.end()) {
        throw SFMLAudioError("Sound '" + soundName + "' not found in audio registry");
    }

    infoIt->second.defaultVolume = volume;

    auto soundIt = _activeSounds.find(soundName);
    if (soundIt != _activeSounds.end() && soundIt->second) {
        soundIt->second->setVolume(volume * (_globalSoundVolume / 100.0f));
    }
}

void SFMLAudio::setMusicVolume(float volume)
{
    if (volume < 0.0f || volume > 100.0f) {
        throw SFMLAudioError("Volume must be between 0.0 and 100.0");
    }

    _musicVolume = volume;
    if (_music) {
        _music->setVolume(_musicVolume);
    }
}

void SFMLAudio::setGlobalSoundVolume(float volume)
{
    if (volume < 0.0f || volume > 100.0f) {
        throw SFMLAudioError("Volume must be between 0.0 and 100.0");
    }

    _globalSoundVolume = volume;

    for (auto &[name, sound] : _activeSounds) {
        if (sound) {
            auto infoIt = _audioInfo.find(name);
            if (infoIt != _audioInfo.end()) {
                sound->setVolume(infoIt->second.defaultVolume * (_globalSoundVolume / 100.0f));
            }
        }
    }

    if (_music) {
        _music->setVolume(_musicVolume * (_globalSoundVolume / 100.0f));
    }
}

float SFMLAudio::getGlobalSoundVolume() const
{
    return _globalSoundVolume;
}

bool SFMLAudio::hasSound(const std::string &soundName) const
{
    return _audioInfo.find(soundName) != _audioInfo.end();
}

bool SFMLAudio::isMusicPlaying() const
{
    return _music && _music->getStatus() == sf::SoundSource::Status::Playing;
}

SoundInfo SFMLAudio::getSoundInfoFromName(const std::string &soundName) const
{
    auto it = _audioInfo.find(soundName);
    if (it != _audioInfo.end()) {
        return it->second;
    }

    throw SFMLAudioError("Sound '" + soundName + "' not found in audio registry");
}

std::vector<std::string> SFMLAudio::listSounds() const
{
    std::vector<std::string> soundList;
    soundList.reserve(_audioInfo.size());
    for (const auto &[name, info] : _audioInfo) {
        soundList.push_back(name);
    }
    return soundList;
}

bool SFMLAudio::loadSoundBuffer(const std::string &soundName)
{
    auto it = _audioInfo.find(soundName);
    if (it == _audioInfo.end()) {
        return false;
    }

    auto resourceData = _resourceManager->loadResource(it->second.path);
    if (resourceData.data == nullptr || resourceData.size == 0) {
        std::cerr << "Warning: Failed to load embedded sound resource '" << it->second.path << "'" << std::endl;
        return false;
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromMemory(resourceData.data, resourceData.size)) {
        std::cerr << "Warning: Failed to load sound buffer from memory for '" << it->second.path << "'" << std::endl;
        return false;
    }

    _soundBuffers[soundName] = std::move(buffer);
    return true;
}
