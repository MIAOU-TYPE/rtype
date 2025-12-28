/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusic
*/

#include "SfmlMusic.hpp"

namespace Graphics
{
    SfmlMusic::SfmlMusic(std::unique_ptr<sf::Music> music) : _music(std::move(music))
    {
    }

    void SfmlMusic::play()
    {
        if (_music)
            _music->play();
    }

    void SfmlMusic::stop()
    {
        if (_music)
            _music->stop();
    }

    void SfmlMusic::pause()
    {
        if (_music)
            _music->pause();
    }

    bool SfmlMusic::isPlaying() const
    {
        return _music && _music->getStatus() == sf::Music::Status::Playing;
    }

    void SfmlMusic::setVolume(float volume)
    {
        if (_music)
            _music->setVolume(volume);
    }

    void SfmlMusic::setLooping(bool loop)
    {
        if (_music)
            _music->setLooping(loop);
    }

    sf::Music *SfmlMusic::get() noexcept
    {
        return _music.get();
    }
} // namespace Graphics
