/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** AudioRegistry
*/

#pragma once

#include <string>
#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>
#include "IAudioManager.hpp"
#include "SfmlSound.hpp"
#include <SFML/Audio.hpp>

namespace Graphics
{
    class SfmlSoundManager;
    class SfmlMusicManager;
}

namespace Engine
{
    class AudioRegistryError : public std::exception {
      public:
        /**
         * @brief Constructs a AudioRegistryError with a given message.
         * @param message Error message describing the issue.
         */
        explicit AudioRegistryError(const std::string &message) : _message("\n\t" + message)
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
     * @class AudioRegistry
     * @brief Manages audio playback for sounds and music.
     */
    class AudioRegistry {
      public:
        using AudioHandle = Graphics::AudioHandle;

        /**
         * @brief Construct a new AudioRegistry object.
         * @param soundManager Shared pointer to the sound manager.
         * @param musicManager Shared pointer to the music manager.
         */
        AudioRegistry(std::shared_ptr<Graphics::IAudioManager> soundManager,
          std::shared_ptr<Graphics::IAudioManager> musicManager);

        /**
         * @brief Play a sound effect.
         * @param handle Handle to the sound to play.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playSound(AudioHandle handle, float volume = 100.f);

        /**
         * @brief Play a music track.
         * @param handle Handle to the music to play.
         * @param loop Whether the music should loop.
         * @param volume Volume level (0.0 to 100.0).
         */
        void playMusic(AudioHandle handle, bool loop = true, float volume = 50.f);

        /**
         * @brief Stop the currently playing music.
         */
        void stopMusic();

        /**
         * @brief Set the volume of the currently playing music.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setMusicVolume(float volume);

        /**
         * @brief Set the global volume for all sound effects.
         * @param volume Volume level (0.0 to 100.0).
         */
        void setSoundVolume(float volume);

      private:
        std::shared_ptr<Graphics::IAudioManager> _soundManager = nullptr;   ///> Sound manager
        std::shared_ptr<Graphics::IAudioManager> _musicManager = nullptr;   ///> Music manager
        
        std::shared_ptr<Graphics::SfmlSoundManager> _sfmlSoundManager = nullptr; ///> Concrete sound manager
        std::shared_ptr<Graphics::SfmlMusicManager> _sfmlMusicManager = nullptr; ///> Concrete music manager

        std::vector<std::unique_ptr<Graphics::SfmlSound>> _activeSounds;    ///> Active sounds being played

        AudioHandle _currentMusicHandle = Graphics::InvalidAudio; ///> Currently playing music handle
        float _globalSoundVolume = 100.f;                                   ///> Global sound volume
    };
} // namespace Engine