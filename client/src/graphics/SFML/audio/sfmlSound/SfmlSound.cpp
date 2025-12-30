/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSound
*/

#include "SfmlSound.hpp"

namespace Graphics
{
    SfmlSound::SfmlSound(
        std::shared_ptr<Resources::IResourceManager> resources, const std::string &resourcePath, float volume)
        : _buffer(std::make_unique<sf::SoundBuffer>()), _sound(*_buffer)
    {
        auto [data, size] = resources->loadResource(resourcePath);
        if (!data || size == 0)
            throw AudioError("SfmlSound: failed to load resource");

        if (!_buffer->loadFromMemory(data, size))
            throw AudioError("SfmlSound: failed to load sound buffer");

        _sound.setVolume(volume);
    }

    void SfmlSound::play()
    {
        _sound.play();
    }

    void SfmlSound::stop()
    {
        _sound.stop();
    }

    bool SfmlSound::isPlaying() const
    {
        return _sound.getStatus() == sf::Sound::Status::Playing;
    }

    void SfmlSound::setVolume(float volume)
    {
        _sound.setVolume(volume);
    }

    void SfmlSound::setLooping(bool loop)
    {
        _sound.setLooping(loop);
    }

    sf::Sound &SfmlSound::get() noexcept
    {
        return _sound;
    }
} // namespace Graphics