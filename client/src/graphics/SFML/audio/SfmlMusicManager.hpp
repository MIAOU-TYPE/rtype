/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlMusicManager
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "IAudioManager.hpp"
#include "IResourceManager.hpp"
#include "SfmlMusic.hpp"

namespace Graphics
{
    /**
     * @class SfmlMusicManager
     * @brief SFML implementation of the IAudioManager interface.
     * This class manages audio loading, playback, and control using SFML.
     */
    class SfmlMusicManager : public IAudioManager {
      public:
        /**
         * @brief Constructor for SfmlMusicManager.
         * @param resources Shared pointer to the resource manager for loading audio data.
         */
        explicit SfmlMusicManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Destructor for SfmlMusicManager.
         */
        ~SfmlMusicManager() override = default;

        /**
         * @brief Load a music track from a resource path.
         * @param resourcePath Path to the music resource.
         * @return AudioHandle Handle to the loaded music, or InvalidAudio on failure.
         */
        AudioHandle loadAudio(const std::string &resourcePath) override;

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
         * @brief Get the music object for a given handle.
         * @param handle The audio handle.
         * @return Pointer to the music object, or nullptr if invalid.
         */
        [[nodiscard]] SfmlMusic *getMusic(const AudioHandle handle) noexcept;

      private:
        /**
         * @struct MusicEntry
         * @brief Represents a loaded music track.
         */
        struct MusicEntry {
            std::unique_ptr<SfmlMusic> music; ///> The SFML music wrapper
            std::string resourcePath;         ///> Path to the music resource
        };

        std::shared_ptr<Resources::IResourceManager> _resources = nullptr; ///> Resource manager

        std::unordered_map<AudioHandle, MusicEntry> _musics;             ///> Map of music handles to music entries
        std::unordered_map<std::string, AudioHandle> _musicPathToHandle; ///> Map of music paths to handles

        AudioHandle _nextHandle = 1; ///> Next available audio handle
    };
} // namespace Graphics
