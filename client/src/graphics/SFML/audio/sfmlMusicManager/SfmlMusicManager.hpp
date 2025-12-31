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
     * @brief SFML implementation of the IAudioManager interface for music.
     * This class manages music loading and playback using SFML.
     */
    class SfmlMusicManager final : public IAudioManager {
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
         * @brief Play the music.
         * @param handle The audio handle.
         */
        void play(AudioHandle handle) override;

        /**
         * @brief Stop the music.
         * @param handle The audio handle.
         */
        void stop(AudioHandle handle) override;

        /**
         * @brief Set the volume of the music.
         * @param handle The audio handle.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setVolume(AudioHandle handle, float volume) override;

        /**
         * @brief Set whether the music should loop.
         * @param handle The audio handle.
         * @param loop True to loop, false otherwise.
         */
        void setLooping(AudioHandle handle, bool loop) override;

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
