/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SfmlAudioManager
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
     * @class SfmlAudioManager
     * @brief SFML implementation of the IAudioManager interface.
     * This class manages audio loading, playback, and control using SFML.
     */
    class SfmlAudioManager : public IAudioManager {
      public:
        /**
         * @brief Constructor for SfmlAudioManager.
         * @param resources Shared pointer to the resource manager for loading audio data.
         */
        explicit SfmlAudioManager(std::shared_ptr<Resources::IResourceManager> resources);

        /**
         * @brief Destructor for SfmlAudioManager.
         */
        ~SfmlAudioManager() override = default;

        /**
         * @brief Load a sound effect from a resource path.
         * @param resourcePath Path to the sound resource.
         * @return AudioHandle Handle to the loaded sound, or InvalidAudio on failure.
         */
        AudioHandle loadSound(const std::string &resourcePath) override;

        /**
         * @brief Load a music track from a resource path.
         * @param resourcePath Path to the music resource.
         * @return AudioHandle Handle to the loaded music, or InvalidAudio on failure.
         */
        AudioHandle loadMusic(const std::string &resourcePath) override;

        /**
         * @brief Play a sound effect.
         * @param handle Handle to the sound to play.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playSound(AudioHandle handle, float volume = 100.f) override;

        /**
         * @brief Play a music track.
         * @param handle Handle to the music to play.
         * @param loop Whether the music should loop.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playMusic(AudioHandle handle, bool loop = true, float volume = 50.f) override;

        /**
         * @brief Stop the currently playing music.
         */
        void stopMusic() override;

        /**
         * @brief Pause the currently playing music.
         */
        void pauseMusic() override;

        /**
         * @brief Resume the paused music.
         */
        void resumeMusic() override;

        /**
         * @brief Set the volume of the currently playing music.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setMusicVolume(float volume) override;

        /**
         * @brief Set the global volume for all sound effects.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setSoundVolume(float volume) override;

        /**
         * @brief Unload a sound or music resource.
         * @param handle Handle to the audio resource to unload.
         */
        void unload(AudioHandle handle) override;

        /**
         * @brief Check if an audio handle is valid.
         * @param handle The audio handle to check.
         * @return True if the handle is valid, false otherwise.
         */
        bool isValid(AudioHandle handle) const noexcept override;

        /**
         * @brief Clear all loaded audio resources.
         */
        void clear() override;

      private:
        /**
         * @struct SoundEntry
         * @brief Represents a loaded sound buffer and its reference count.
         */
        struct SoundEntry {
            sf::SoundBuffer buffer;                                         ///> The SFML sound buffer
            std::size_t refCount = 1;                                       ///> Reference count for the sound
        };

        /**
         * @struct MusicEntry
         * @brief Represents a loaded music track.
         */
        struct MusicEntry {
            std::unique_ptr<sf::Music> music;                               ///> The SFML music object
            std::string resourcePath;                                       ///> Path to the music resource
        };

        std::shared_ptr<Resources::IResourceManager> _resources = nullptr;  ///> Resource manager

        std::unordered_map<AudioHandle, SoundEntry> _sounds;                ///> Map of sound handles to sound entries
        std::unordered_map<std::string, AudioHandle> _soundPathToHandle;    ///> Map of sound paths to handles

        std::unordered_map<AudioHandle, MusicEntry> _musics;                ///> Map of music handles to music entries
        std::unordered_map<std::string, AudioHandle> _musicPathToHandle;    ///> Map of music paths to handles

        AudioHandle _nextHandle = 1;                                        ///> Next available audio handle
        AudioHandle _currentMusicHandle = InvalidAudio;                     ///> Currently playing music handle
        float _globalSoundVolume = 100.f;                                   ///> Global sound volume
        std::vector<std::unique_ptr<SfmlSound>> _activeSounds;             ///> Active sounds being played
    };
} // namespace Graphics
