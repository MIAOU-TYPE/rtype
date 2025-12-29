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
     * @brief SFML implementation of the IAudioManager interface.
     * This class manages audio loading, playback, and control using SFML.
     */
    class SfmlSoundManager : public IAudioManager {
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
        AudioHandle load(const std::string &resourcePath) override;

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
        bool isValid(const AudioHandle handle) const noexcept override;

        /**
         * @brief Clear all loaded audio resources.
         */
        void clear() override;

        /**
         * @brief Get the sound buffer for a given handle.
         * @param handle The audio handle.
         * @return Pointer to the sound buffer, or nullptr if invalid.
         */
        [[nodiscard]] const sf::SoundBuffer *getSoundBuffer(const AudioHandle handle) const noexcept;

      private:
        /**
         * @struct SoundEntry
         * @brief Represents a loaded sound buffer and its reference count.
         */
        struct SoundEntry {
            sf::SoundBuffer buffer;   ///> The SFML sound buffer
            std::size_t refCount = 1; ///> Reference count for the sound
        };

        std::shared_ptr<Resources::IResourceManager> _resources = nullptr; ///> Resource manager

        std::unordered_map<AudioHandle, SoundEntry> _sounds;             ///> Map of sound handles to sound entries
        std::unordered_map<std::string, AudioHandle> _soundPathToHandle; ///> Map of sound paths to handles

        AudioHandle _nextHandle = 1; ///> Next available audio handle
    };
} // namespace Graphics
