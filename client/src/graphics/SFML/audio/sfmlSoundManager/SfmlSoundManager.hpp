/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSoundManager
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "IAudioManager.hpp"
#include "IResourceManager.hpp"
#include "SfmlSound.hpp"

namespace Graphics
{
    /**
     * @class SfmlSoundManager
     * @brief SFML implementation of the IAudioManager interface for sound effects.
     * This class manages sound loading and playback using SFML.
     */
    class SfmlSoundManager final : public IAudioManager {
      public:
        /**
         * @brief Constructor for SfmlSoundManager.
         * @param resources Shared pointer to the resource manager for loading audio data.
         */
        explicit SfmlSoundManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Destructor for SfmlSoundManager.
         */
        ~SfmlSoundManager() override = default;

        /**
         * @brief Load a sound effect from a resource path.
         * @param resourcePath Path to the sound resource.
         * @return AudioHandle Handle to the loaded sound, or InvalidAudio on failure.
         */
        [[nodiscard]] AudioHandle load(const std::string &resourcePath) override;

        /**
         * @brief Unload a sound or music resource.
         * @param handle Handle to the audio resource to unload.
         */
        void unload(const AudioHandle handle) override;

        /**
         * @brief Check if an audio handle is valid.
         * @param handle The audio handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool isValid(const AudioHandle handle) const noexcept override;

        /**
         * @brief Clear all loaded audio resources.
         */
        void clear() override;

        /**
         * @brief Play the sound.
         * @param handle The audio handle.
         */
        bool play(AudioHandle handle) override;

        /**
         * @brief Stop the sound.
         * @param handle The audio handle.
         */
        void stop(AudioHandle handle) override;

        /**
         * @brief Set the volume of the sound.
         * @param handle The audio handle.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(AudioHandle handle, float volume) override;

        /**
         * @brief Set whether the sound should loop.
         * @param handle The audio handle.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(AudioHandle handle, bool loop) override;

        /**
         * @brief Set the global volume multiplier for all active audio.
         * @param multiplier Volume multiplier (e.g., 0.5 for half volume).
         */
        void setGlobalVolume(float multiplier) override;

      private:
        /**
         * @struct ActiveSound
         * @brief Represents an active playing sound.
         */
        struct ActiveSound {
            AudioHandle handle;               ///< Handle to the audio resource.
            std::unique_ptr<SfmlSound> sound; ///< Unique pointer to the SFML sound object.
        };

        /**
         * @struct SoundEntry
         * @brief Represents a loaded sound.
         */
        struct SoundEntry {
            std::string resourcePath; ///> Path to the sound resource
        };

        std::shared_ptr<Resources::IResourceManager> _resources = nullptr; ///> Resource manager

        std::unordered_map<AudioHandle, SoundEntry> _sounds;             ///> Map of sound handles to sound entries
        std::unordered_map<std::string, AudioHandle> _soundPathToHandle; ///> Map of sound paths to handles
        std::vector<ActiveSound> _activeSounds;                          ///> List of active playing sounds

        AudioHandle _nextHandle = 1; ///> Next available audio handle
    };
} // namespace Graphics
