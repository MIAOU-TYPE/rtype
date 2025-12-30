/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSound
*/

#include "SfmlSound.hpp"

namespace Graphics
{
    SfmlSound::SfmlSound(std::shared_ptr<sf::SoundBuffer> buffer, float volume)
        : _buffer(std::move(buffer)), _sound(*_buffer)
    {
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