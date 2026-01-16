/*
** EPITECH PROJECT, 2026
** rtype
** File description:
** SettingsConfig
*/

#pragma once

#include <atomic>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "GraphicsTypes.hpp"
#include "Key.hpp"
#include <unordered_map>

/**
 * @brief Type alias for nlohmann::json for convenience.
 */
using json = nlohmann::json;

namespace Utils
{
    /**
     * @enum KeyPreset
     * @brief Enumeration of available keyboard presets
     */
    enum class KeyPreset { Arrows, ZQSD, Custom };

    /**
     * @enum BindAction
     * @brief Enumeration of bindable actions
     */
    enum class BindAction { Up, Down, Left, Right, Shoot };

    /**
     * @brief Structure holding movement key bindings
     */
    struct MovementKeys {
        Engine::Key up;    ///> Key for moving up
        Engine::Key down;  ///> Key for moving down
        Engine::Key left;  ///> Key for moving left
        Engine::Key right; ///> Key for moving right
        Engine::Key shoot; ///> Key for shooting
    };

    /**
     * @class SettingsConfig
     * @brief Manages application settings configuration.
     */
    class SettingsConfig {
      public:
        /**
         * @brief Get the singleton instance
         * @return Reference to the SettingsConfig instance
         */
        static SettingsConfig &getInstance();

        /**
         * @brief Destructor for SettingsConfig.
         */
        ~SettingsConfig();

        /**
         * @brief Save settings if there are unsaved changes
         * @return True if no changes to save or saving was successful, false otherwise.
         */
        [[nodiscard]] bool saveIfDirty() noexcept;

        /**
         * @brief Loads settings from the configuration file.
         * @return True if loading was successful, false otherwise.
         */
        [[nodiscard]] bool load() noexcept;

        /**
         * @brief Saves settings to the configuration file.
         * @return True if saving was successful, false otherwise.
         */
        [[nodiscard]] bool save() const noexcept;

        /**
         * @brief Get the Music Volume object
         * @return size_t
         */
        [[nodiscard]] size_t getMusicVolume() const noexcept;

        /**
         * @brief Get the Sfx Volume object
         * @return size_t
         */
        [[nodiscard]] size_t getSfxVolume() const noexcept;

        /**
         * @brief Get the Music Muted object
         * @return bool
         */
        [[nodiscard]] bool isMusicMuted() const noexcept;

        /**
         * @brief Get the Sfx Muted object
         * @return bool
         */
        [[nodiscard]] bool isSfxMuted() const noexcept;

        /**
         * @brief Get the Resolution object
         * @return Graphics::Extent2u
         */
        [[nodiscard]] Graphics::Extent2u getResolution() const noexcept;

        /**
         * @brief Get the Color Blind Mode object
         * @return Graphics::ColorBlindMode
         */
        [[nodiscard]] Graphics::ColorBlindMode getColorBlindMode() const noexcept;

        /**
         * @brief Get the current key preset
         * @return The current KeyPreset
         */
        [[nodiscard]] KeyPreset getCurrentPreset() const noexcept;

        /**
         * @brief Get the movement keys for the current preset
         * @return MovementKeys structure with current bindings
         */
        [[nodiscard]] MovementKeys getMovementKeys() const noexcept;

        /**
         * @brief Get the name of a preset
         * @param preset The preset to get the name for
         * @return String name of the preset
         */
        [[nodiscard]] static std::string getPresetName(KeyPreset preset);

        /**
         * @brief Convert a key to string
         * @param key The key to convert
         * @return String representation of the key
         */
        [[nodiscard]] static std::string keyToString(Engine::Key key);

        /**
         * @brief Get a key binding for a specific action
         * @param action The action to get the key for
         * @return The key bound to the action
         */
        [[nodiscard]] Engine::Key getKey(BindAction action) const noexcept;

        /**
         * @brief Set a key binding for a specific action
         * @param action The action to bind
         * @param key The key to bind to the action
         */
        void setKey(BindAction action, Engine::Key key);

        /**
         * @brief Check if controls need to be rebound
         * @return true if rebinding is needed, false otherwise
         */
        [[nodiscard]] bool needsRebind() const noexcept;

        /**
         * @brief Clear the rebind flag
         */
        void clearRebindFlag() noexcept;

        /**
         * @brief Check if a key is already assigned to another movement command
         * @param key The key to check
         * @param excludeCommand The command to exclude from the check (optional)
         * @return true if the key is already assigned, false otherwise
         */
        [[nodiscard]] bool isKeyAlreadyAssigned(
            Engine::Key key, Engine::Key excludeCommand = Engine::Key::Unknown) const noexcept;

        /**
         * @brief Set the Music Volume object
         * @param volume
         */
        void setMusicVolume(size_t volume);

        /**
         * @brief Set the Sfx Volume object
         * @param volume
         */
        void setSfxVolume(size_t volume);

        /**
         * @brief Set the Music Muted object
         * @param muted
         */
        void setMusicMuted(bool muted);

        /**
         * @brief Set the Sfx Muted object
         * @param muted
         */
        void setSfxMuted(bool muted);

        /**
         * @brief Set the Resolution object
         * @param resolution
         */
        void setResolution(Graphics::Extent2u resolution);

        /**
         * @brief Set the Color Blind Mode object
         * @param mode
         */
        void setColorBlindMode(Graphics::ColorBlindMode mode);

        /**
         * @brief Set the current key preset
         * @param preset The preset to set
         */
        void setPreset(KeyPreset preset);

        /**
         * @brief Set custom movement keys
         * @param keys The custom keys
         */
        void setCustomMovementKeys(MovementKeys keys);

        /**
         * @brief Delete copy constructor
         */
        SettingsConfig(const SettingsConfig &) = delete;

        /**
         * @brief Delete assignment operator
         */
        SettingsConfig &operator=(const SettingsConfig &) = delete;

      private:
        /**
         * @brief Constructor for SettingsConfig.
         */
        SettingsConfig();

        static constexpr const char *CONFIG_FILE =
            "client/config/settings.json"; ///> Path to the settings configuration file

        size_t _musicVolume = 50;                                             ///> Music volume level (0-100)
        size_t _sfxVolume = 50;                                               ///> SFX volume level (0-100)
        bool _musicMuted = false;                                                  ///> Music mute state
        bool _sfxMuted = false;                                                    ///> SFX mute state
        Graphics::Extent2u _resolution{1280, 720};                                 ///> Screen resolution
        Graphics::ColorBlindMode _colorBlindMode = Graphics::ColorBlindMode::NONE; ///> Colorblind mode

        KeyPreset _currentPreset = KeyPreset::Arrows;         ///> Current key preset
        std::unordered_map<KeyPreset, MovementKeys> _presets; ///> Map of presets to movement keys
        MovementKeys _customKeys;                             ///> Custom key bindings
        std::atomic<bool> _needsRebind = false;               ///> Flag indicating if rebinding is needed
        mutable bool _dirty = false;                          ///> Flag indicating unsaved changes
    };
} // namespace Utils