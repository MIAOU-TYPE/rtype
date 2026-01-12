/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlSoundManager
*/

#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
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
         * @param audioHandle Handle to the audio resource to unload.
         */
        void unload(AudioHandle audioHandle) override;

        /**
         * @brief Check if an audio handle is valid.
         * @param audioHandle The audio handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool isValid(AudioHandle audioHandle) const noexcept override;

        /**
         * @brief Clear all loaded audio resources.
         */
        void clear() override;

        /**
         * @brief Play the sound.
         * @param audioHandle The audio handle.
         */
        bool play(AudioHandle audioHandle) override;

        /**
         * @brief Stop the sound.
         * @param audioHandle The audio handle.
         */
        void stop(AudioHandle audioHandle) override;

        /**
         * @brief Set the volume of the sound.
         * @param audioHandle The audio handle.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(AudioHandle audioHandle, float volume) override;

        /**
         * @brief Set whether the sound should loop.
         * @param audioHandle The audio handle.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(AudioHandle audioHandle, bool loop) override;

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
            float volume = 100.f;     ///> Volume for this sound (0-100)
        };

        std::shared_ptr<Resources::IResourceManager> _resources = nullptr; ///> Resource manager

        std::unordered_map<AudioHandle, SoundEntry> _sounds;             ///> Map of sound handles to sound entries
        std::unordered_map<std::string, AudioHandle> _soundPathToHandle; ///> Map of sound paths to handles
        std::vector<ActiveSound> _activeSounds;                          ///> List of active playing sounds

        AudioHandle _nextHandle = 1; ///> Next available audio handle
    };
} // namespace Graphics
