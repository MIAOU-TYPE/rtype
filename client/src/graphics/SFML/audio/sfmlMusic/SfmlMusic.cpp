/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusic
*/

#include "SfmlMusic.hpp"

namespace Graphics
{
    SfmlMusic::SfmlMusic(std::shared_ptr<Resources::IResourceManager> resources, const std::string &resourcePath)
    {
        auto [data, size] = resources->loadResource(resourcePath);
        if (!data || size == 0)
            throw AudioError("SfmlMusic: failed to load resource");

        _music = std::make_unique<sf::Music>();
        if (!_music->openFromMemory(data, size))
            throw AudioError("SfmlMusic: failed to open music from memory");
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
} // namespace Graphics
