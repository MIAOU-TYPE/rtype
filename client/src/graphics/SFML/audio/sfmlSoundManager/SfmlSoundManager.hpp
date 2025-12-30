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
#include "IAudioPlayable.hpp"
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
         * @brief Create a new sound instance from a loaded sound buffer.
         * @param handle The audio handle.
         * @param volume Initial volume level (0.0 to 100.0).
         * @return Unique pointer to the sound, or nullptr if invalid.
         */
        [[nodiscard]] std::unique_ptr<IAudioPlayable> createSound(
            const AudioHandle handle, float volume = 100.f) const noexcept override;

        /**
         * @brief Not supported for sound manager.
         * @return Always returns nullptr.
         */
        [[nodiscard]] std::shared_ptr<IAudioPlayable> get(AudioHandle) noexcept override;

      private:
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

        AudioHandle _nextHandle = 1; ///> Next available audio handle
    };
} // namespace Graphics
