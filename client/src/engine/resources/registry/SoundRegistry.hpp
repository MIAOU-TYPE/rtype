/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SoundRegistry
*/

#pragma once

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include "IAudioManager.hpp"
#include "IAudioPlayable.hpp"

namespace Engine
{
    /**
     * @class SoundRegistryError
     * @brief Exception class for SoundRegistry errors.
     */
    class SoundRegistryError : public std::exception {
      public:
        /**
         * @brief Constructs a SoundRegistryError with a given message.
         * @param message Error message describing the issue.
         */
        explicit SoundRegistryError(const std::string &message) : _message("\n\t" + message)
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
     * @class SoundRegistry
     * @brief Manages sound effect playback.
     */
    class SoundRegistry {
      public:
        /**
         * @brief Type alias for audio handle used to reference sound resources.
         */
        using AudioHandle = Graphics::AudioHandle;

        /**
         * @brief Construct a new SoundRegistry object.
         * @param soundManager Shared pointer to the audio manager.
         */
        explicit SoundRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager);

        /**
         * @brief Play a sound effect.
         * @param handle Handle to the sound to play.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playSound(AudioHandle handle, float volume = 100.f);

        /**
         * @brief Set the global volume for all sound effects.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setSoundVolume(float volume);

      private:
        std::shared_ptr<Graphics::IAudioManager> _soundManager = nullptr; ///> Sound manager

        std::vector<std::unique_ptr<Graphics::IAudioPlayable>> _activeSounds; ///> Active sounds being played
        float _globalSoundVolume = 100.f;                                    ///> Global sound volume
    };
} // namespace Engine
