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

namespace Engine
{
    /**
     * @brief Alias for invalid audio handle
     */
    using Graphics::InvalidAudio;

    /**
     * @class MusicRegistryError
     * @brief Exception class for MusicRegistry errors.
     */
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
        /**
         * @brief Type alias for audio handle used to reference music resources.
         */
        using AudioHandle = Graphics::AudioHandle;

        /**
         * @brief Construct a new MusicRegistry object.
         * @param musicManager Shared pointer to the audio manager.
         */
        explicit MusicRegistry(std::shared_ptr<Graphics::IAudioManager> musicManager);

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

        /**
         * @brief Get the current music volume.
         * @return Volume level (0.0 to 100.0).
         */
        [[nodiscard]] float getMusicVolume() const noexcept;

        /**
         * @brief Set the volume before mute.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolumeBeforeMute(float volume);

        /**
         * @brief Get the volume before mute.
         * @return Volume level (0.0 to 100.0).
         */
        [[nodiscard]] float getVolumeBeforeMute() const noexcept;

        /**
         * @brief Load and play a music track from file.
         * @param path Path to the music file.
         * @param loop Whether the music should loop.
         * @param volume Volume level (0.0 to 100.0).
         * @return true if the music was loaded and started playing, false otherwise.
         */
        [[nodiscard]] bool loadAndPlayMusic(const std::string &path, bool loop = true, float volume = 50.f) noexcept;

      private:
        std::shared_ptr<Graphics::IAudioManager> _musicManager = nullptr; ///> Music manager

        AudioHandle _currentMusicHandle = Graphics::InvalidAudio; ///> Currently playing music handle
        float _globalMusicVolume = 50.f;                          ///> Global music volume
        float _volumeBeforeMute = 50.f;                           ///> Volume before mute
    };
} // namespace Engine
