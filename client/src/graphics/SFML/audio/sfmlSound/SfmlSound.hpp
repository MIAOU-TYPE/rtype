/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSound
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include "IAudioManager.hpp"
#include "IAudioPlayable.hpp"
#include "IResourceManager.hpp"

namespace Graphics
{
    /**
     * @class SfmlSound
     * @brief Wrapper class for managing an active SFML sound.
     * This class handles the lifecycle of a playing sound effect.
     */
    class SfmlSound final : public IAudioPlayable {
      public:
        /**
         * @brief Constructor for SfmlSound.
         * @param resources Shared pointer to the resource manager.
         * @param resourcePath Path to the sound resource.
         * @param volume Volume level (0.0 to 100.0).
         */
        explicit SfmlSound(std::shared_ptr<Resources::IResourceManager> resources, const std::string &resourcePath,
            float volume = 100.f);

        /**
         * @brief Play the sound.
         */
        void play() override;

        /**
         * @brief Stop the sound.
         */
        void stop() override;

        /**
         * @brief Check if the sound is currently playing.
         * @return True if playing, false otherwise.
         */
        [[nodiscard]] bool isPlaying() const override;

        /**
         * @brief Set the volume of the sound.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(float volume) override;

        /**
         * @brief Set whether the sound should loop.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(bool loop) override;

        /**
         * @brief Get the underlying SFML sound object.
         * @return Reference to the sf::Sound object.
         */
        [[nodiscard]] sf::Sound &get() noexcept;

      private:
        std::unique_ptr<sf::SoundBuffer> _buffer; ///> Sound buffer
        sf::Sound _sound;                         ///> SFML sound object
    };
} // namespace Graphics