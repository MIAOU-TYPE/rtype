/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MusicRegistry
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include "IAudioManager.hpp"

namespace Graphics
{
    class SfmlMusicManager;
}

namespace Engine
{
    class MusicRegistryError : public std::exception {
      public:
        /**
         * @brief Constructs a MusicRegistryError with a given message.
         * @param message Error message describing the issue.
         */
        explicit MusicRegistryError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Returns the error message.
         * @return C-string representing the error message.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class MusicRegistry
     * @brief Manages music playback.
     */
    class MusicRegistry {
      public:
        using AudioHandle = Graphics::AudioHandle;

        /**
         * @brief Construct a new MusicRegistry object.
         * @param musicManager Shared pointer to the SFML music manager.
         */
        explicit MusicRegistry(std::shared_ptr<Graphics::SfmlMusicManager> musicManager);

        /**
         * @brief Play a music track.
         * @param handle Handle to the music to play.
         * @param loop Whether the music should loop.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playMusic(AudioHandle handle, bool loop = true, float volume = 50.f);

        /**
         * @brief Stop the currently playing music.
         */
        void stopMusic();

        /**
         * @brief Set the volume of the currently playing music.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setMusicVolume(float volume);

      private:
        std::shared_ptr<Graphics::SfmlMusicManager> _musicManager = nullptr; ///> Music manager

        AudioHandle _currentMusicHandle = Graphics::InvalidAudio; ///> Currently playing music handle
    };
} // namespace Engine
