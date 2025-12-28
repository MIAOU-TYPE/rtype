/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusic
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>

namespace Graphics
{
    /**
     * @class SfmlMusic
     * @brief Wrapper class for managing SFML music.
     * This class handles the lifecycle of a music track.
     */
    class SfmlMusic {
      public:
        /**
         * @brief Constructor for SfmlMusic.
         * @param music Unique pointer to the music object.
         */
        explicit SfmlMusic(std::unique_ptr<sf::Music> music);

        /**
         * @brief Play the music.
         */
        void play();

        /**
         * @brief Stop the music.
         */
        void stop();

        /**
         * @brief Pause the music.
         */
        void pause();

        /**
         * @brief Check if the music is currently playing.
         * @return True if playing, false otherwise.
         */
        [[nodiscard]] bool isPlaying() const;

        /**
         * @brief Set the volume of the music.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(float volume);

        /**
         * @brief Set whether the music should loop.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(bool loop);

        /**
         * @brief Get the underlying SFML music object.
         * @return Pointer to the sf::Music object.
         */
        [[nodiscard]] sf::Music *get() noexcept;

      private:
        std::unique_ptr<sf::Music> _music; ///> SFML music object
    };
} // namespace Graphics
