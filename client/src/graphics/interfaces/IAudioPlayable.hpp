/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** IAudioPlayable
*/

#pragma once

namespace Graphics
{
    /**
     * @class IAudioPlayable
     * @brief Base interface for audio playback.
     * This interface defines common operations for all audio types.
     */
    class IAudioPlayable {
      public:
        /**
         * @brief Virtual destructor for IAudioPlayable.
         */
        virtual ~IAudioPlayable() = default;

        /**
         * @brief Play the audio.
         */
        virtual void play() = 0;

        /**
         * @brief Stop the audio.
         */
        virtual void stop() = 0;

        /**
         * @brief Check if the audio is currently playing.
         * @return True if playing, false otherwise.
         */
        [[nodiscard]] virtual bool isPlaying() const = 0;

        /**
         * @brief Set the volume of the audio.
         * @param volume Volume level (0.0 to 100.0).
         */
        virtual void setVolume(float volume) = 0;

        /**
         * @brief Set whether the audio should loop.
         * @param loop True to loop, false otherwise.
         */
        virtual void setLooping(bool loop) = 0;
    };
} // namespace Graphics
