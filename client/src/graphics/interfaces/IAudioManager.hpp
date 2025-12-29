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
    class AudioError : public std::exception {
      public:
        /**
         * @brief Constructor for AudioError.
         * @param message The error message.
         */
        explicit AudioError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Override of what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message; ///> Error message
    };

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
         * @brief Load an audio resource (sound or music) from a resource path.
         * @param resourcePath Path to the audio resource.
         * @return AudioHandle Handle to the loaded audio, or InvalidAudio on failure.
         */
        [[nodiscard]] virtual AudioHandle load(const std::string &resourcePath) = 0;

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
