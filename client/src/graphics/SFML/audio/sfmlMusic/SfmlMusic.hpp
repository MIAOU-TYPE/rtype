/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusic
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include "IAudioPlayable.hpp"
#include "IResourceManager.hpp"
#include "IAudioManager.hpp"

namespace Graphics
{
    /**
     * @class SfmlMusic
     * @brief Wrapper class for managing SFML music.
     * This class handles the lifecycle of a music track.
     */
    class SfmlMusic final : public IAudioPlayable {
      public:
        /**
         * @brief Constructor for SfmlMusic.
         * @param resources Shared pointer to the resource manager.
         * @param resourcePath Path to the music resource.
         */
        explicit SfmlMusic(std::shared_ptr<Resources::IResourceManager> resources, const std::string &resourcePath);

        /**
         * @brief Play the music.
         */
        void play() override;

        /**
         * @brief Stop the music.
         */
        void stop() override;

        /**
         * @brief Check if the music is currently playing.
         * @return True if playing, false otherwise.
         */
        [[nodiscard]] bool isPlaying() const override;

        /**
         * @brief Set the volume of the music.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(float volume) override;

        /**
         * @brief Set whether the music should loop.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(bool loop) override;

        /**
         * @brief Get the underlying SFML music object.
         * @return Reference to the sf::Music object.
         */
        [[nodiscard]] sf::Music &get() noexcept;

      private:
        std::unique_ptr<sf::Music> _music; ///> SFML music object
    };
} // namespace Graphics
