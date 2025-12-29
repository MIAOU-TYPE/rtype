/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusic
*/

#include "SfmlMusic.hpp"
#include <stdexcept>

namespace Graphics
{
    SfmlMusic::SfmlMusic(std::unique_ptr<sf::Music> music) : _music(std::move(music))
    {
        if (!_music)
            throw std::invalid_argument("SfmlMusic: music cannot be null");
    }

    void SfmlMusic::play()
    {
        _music->play();
    }

    void SfmlMusic::stop()
    {
        _music->stop();
    }

    bool SfmlMusic::isPlaying() const
    {
        return _music->getStatus() == sf::Music::Status::Playing;
    }

    void SfmlMusic::setVolume(float volume)
    {
        _music->setVolume(volume);
    }

    void SfmlMusic::setLooping(bool loop)
    {
        _music->setLooping(loop);
    }

    sf::Music &SfmlMusic::get() noexcept
    {
        return *_music;
    }
} // namespace Graphics
