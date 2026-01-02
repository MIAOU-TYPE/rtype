/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** InputConfig
*/

#pragma once

#include "Key.hpp"
#include <string>
#include <unordered_map>

namespace Utils
{
    /**
     * @enum KeyPreset
     * @brief Enumeration of available keyboard presets
     */
    enum class KeyPreset { Arrows, ZQSD };

    /**
     * @brief Structure holding movement key bindings
     */
    struct MovementKeys {
        Engine::Key up;
        Engine::Key down;
        Engine::Key left;
        Engine::Key right;
    };

    /**
     * @brief Class managing input configuration and key bindings
     */
    class InputConfig {
      public:
        /**
         * @brief Get the singleton instance
         * @return Reference to the InputConfig instance
         */
        static InputConfig &getInstance();

        /**
         * @brief Get the current key preset
         * @return The current KeyPreset
         */
        [[nodiscard]] KeyPreset getCurrentPreset() const noexcept;

        /**
         * @brief Set the current key preset
         * @param preset The preset to set
         */
        void setPreset(KeyPreset preset);

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
         * @brief Load configuration from file
         * @return true if successful, false otherwise
         */
        bool load();

        /**
         * @brief Save configuration to file
         * @return true if successful, false otherwise
         */
        bool save() const;

        /**
         * @brief Check if controls need to be rebound
         * @return true if rebinding is needed, false otherwise
         */
        [[nodiscard]] bool needsRebind() const noexcept;

        /**
         * @brief Clear the rebind flag
         */
        void clearRebindFlag() noexcept;

      private:
        InputConfig();                                                 ///> Private constructor for singleton pattern
        ~InputConfig() = default;                                      ///> Default destructor
        InputConfig(const InputConfig &) = delete;                     ///> Delete copy constructor
        InputConfig &operator=(const InputConfig &) = delete;          ///> Delete copy assignment operator

        KeyPreset _currentPreset = KeyPreset::Arrows;                  ///> Current key preset
        std::unordered_map<KeyPreset, MovementKeys> _presets;          ///> Map of presets to movement keys
        mutable bool _needsRebind = false;                             ///> Flag indicating if rebinding is needed

        static constexpr const char *CONFIG_FILE = "input_config.cfg"; ///> Configuration file path
    };
} // namespace Utils
