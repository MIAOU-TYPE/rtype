/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IAudioManager
*/

#pragma once

#include <string>

namespace Graphics
{
    /**
     * @brief Handle type for audio resources (music and sounds)
     */
    using AudioHandle = unsigned int;

    /**
     * @brief Invalid audio handle constant
     */
    constexpr AudioHandle InvalidAudio = 0;

    /**
     * @class IAudioManager
     * @brief Interface for audio management.
     * This interface defines operations for loading, playing, and controlling audio.
     */
    class IAudioManager {
      public:
        /**
         * @brief Virtual destructor for IAudioManager.
         */
        virtual ~IAudioManager() = default;

        /**
         * @brief Load a sound effect from a resource path.
         * @param resourcePath Path to the sound resource.
         * @return AudioHandle Handle to the loaded sound, or InvalidAudio on failure.
         */
        [[nodiscard]] virtual AudioHandle loadSound(const std::string &resourcePath) = 0;

        /**
         * @brief Load a music track from a resource path.
         * @param resourcePath Path to the music resource.
         * @return AudioHandle Handle to the loaded music, or InvalidAudio on failure.
         */
        [[nodiscard]] virtual AudioHandle loadMusic(const std::string &resourcePath) = 0;

        /**
         * @brief Play a sound effect.
         * @param handle Handle to the sound to play.
         * @param volume Volume level (0.0 to 100.0).
         */
        virtual void playSound(AudioHandle handle, float volume = 100.f) = 0;

        /**
         * @brief Play a music track.
         * @param handle Handle to the music to play.
         * @param loop Whether the music should loop.
         * @param volume Volume level (0.0 to 100.0).
         */
        virtual void playMusic(AudioHandle handle, bool loop = true, float volume = 50.f) = 0;

        /**
         * @brief Stop the currently playing music.
         */
        virtual void stopMusic() = 0;

        /**
         * @brief Pause the currently playing music.
         */
        virtual void pauseMusic() = 0;

        /**
         * @brief Resume the paused music.
         */
        virtual void resumeMusic() = 0;

        /**
         * @brief Set the volume of the currently playing music.
         * @param volume Volume level (0.0 to 100.0).
         */
        virtual void setMusicVolume(float volume) = 0;

        /**
         * @brief Set the global volume for all sound effects.
         * @param volume Volume level (0.0 to 100.0).
         */
        virtual void setSoundVolume(float volume) = 0;

        /**
         * @brief Unload a sound or music resource.
         * @param handle Handle to the audio resource to unload.
         */
        virtual void unload(AudioHandle handle) = 0;

        /**
         * @brief Check if an audio handle is valid.
         * @param handle The audio handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] virtual bool isValid(AudioHandle handle) const noexcept = 0;

        /**
         * @brief Clear all loaded audio resources.
         */
        virtual void clear() = 0;
    };
} // namespace Graphics
