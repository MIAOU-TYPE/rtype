/*
** EPITECH PROJECT, 2025
** real_r_type
** File description:
** SFMLAudio
*/

#pragma once

#include <SFML/Audio.hpp>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @namespace Audio
 * @brief Contains all audio-related classes and interfaces.
 */
namespace Audio
{
    /**
     * @enum AudioType
     * @brief Enumeration for audio types.
     */
    enum class AudioType { Sound, Music };

    /**
     * @brief Structure containing sound/music information.
     */
    struct SoundInfo {
        std::string path;    ///> Path to the audio file
        AudioType type;      ///> Type of audio (Sound or Music)
        float defaultVolume; ///> Default volume (0.0f to 100.0f)
        bool loop;           ///> Whether the audio should loop
    };

    /**
     * @class SFMLAudioError
     * @brief Exception class for SFML audio-related errors.
     * Inherits from std::exception to provide error handling capabilities.
     */
    class SFMLAudioError : public std::exception {
      public:
        /**
         * @brief Constructor for SFMLAudioError.
         * @param message The error message to be associated with the exception.
         */
        explicit SFMLAudioError(const std::string &message) : _message("\n\t" + message)
        {
        }

        /**
         * @brief Retrieves the error message.
         * @return A C-style string representing the error message.
         */
        const char *what() const noexcept override
        {
            return (_message).c_str();
        }

      private:
        std::string _message = ""; ///> Error message
    };

    /**
     * @class SFMLAudio
     * @brief Manager for playing sounds and music using SFML.
     *
     * This class manages the loading and playing of audio resources (sounds and music).
     */
    class SFMLAudio {
      public:
        /**
         * @brief Constructor for SFMLAudio.
         */
        SFMLAudio();

        /**
         * @brief Destructor for SFMLAudio.
         */
        ~SFMLAudio() = default;

        /**
         * @brief Plays a sound by name.
         * @param soundName The name of the sound to play.
         * @throws SFMLAudioError if the sound is not found.
         */
        void playSound(const std::string &soundName);

        /**
         * @brief Plays music by name.
         * @param musicName The name of the music to play.
         * @param loop Whether the music should loop (default: true).
         * @throws SFMLAudioError if the music is not found.
         */
        void playMusic(const std::string &musicName, bool loop = true);

        /**
         * @brief Stops the currently playing music.
         */
        void stopMusic();

        /**
         * @brief Pauses the currently playing music.
         */
        void pauseMusic();

        /**
         * @brief Resumes the paused music.
         */
        void resumeMusic();

        /**
         * @brief Sets the volume for a specific sound.
         * @param soundName The name of the sound.
         * @param volume Volume level (0.0f to 100.0f).
         */
        void setSoundVolume(const std::string &soundName, float volume);

        /**
         * @brief Sets the music volume.
         * @param volume Volume level (0.0f to 100.0f).
         */
        void setMusicVolume(float volume);

        /**
         * @brief Sets the global volume for all sounds.
         * @param volume Volume level (0.0f to 100.0f).
         */
        void setGlobalSoundVolume(float volume);

        /**
         * @brief Checks if a sound exists in the registry.
         * @param soundName The name of the sound.
         * @return True if the sound exists, false otherwise.
         */
        bool hasSound(const std::string &soundName) const;

        /**
         * @brief Checks if music is currently playing.
         * @return True if music is playing, false otherwise.
         */
        bool isMusicPlaying() const;

        /**
         * @brief Gets the sound info from sound name.
         * @param soundName The name of the sound.
         * @return The sound information.
         * @throws SFMLAudioError if the sound name is not found.
         */
        SoundInfo getSoundInfoFromName(const std::string &soundName) const;

        /**
         * @brief Lists all available sounds.
         * @return Vector of sound names.
         */
        std::vector<std::string> listSounds() const;

      private:
        /**
         * @brief Registers all available sounds and music.
         */
        void registerAudioResources();

        /**
         * @brief Loads a sound buffer from file.
         * @param soundName The name of the sound.
         * @return True if loaded successfully, false otherwise.
         */
        bool loadSoundBuffer(const std::string &soundName);

        std::unordered_map<std::string, SoundInfo> _audioInfo;                     ///> Mapping sound names to info
        std::unordered_map<std::string, sf::SoundBuffer> _soundBuffers;            ///> Loaded sound buffers
        std::unordered_map<std::string, std::unique_ptr<sf::Sound>> _activeSounds; ///> Currently playing sounds
        std::unique_ptr<sf::Music> _music;                                         ///> Music player
        float _globalSoundVolume = 100.0f;                                         ///> Global sound volume
        float _musicVolume = 100.0f;                                               ///> Music volume
    };

} // namespace Audio
