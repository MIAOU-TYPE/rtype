/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#pragma once

#include <algorithm>
#include <fstream>
#include <memory>
#include <stdexcept>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "MusicRegistry.hpp"
#include "RenderCommand.hpp"
#include "SoundRegistry.hpp"
#include "UIButton.hpp"

namespace Engine
{
    /**
     * @brief Exception class for settings menu errors.
     */
    class SettingsMenuError : public std::runtime_error {
      public:
        /**
         * @brief Constructor for SettingsMenuError.
         * @param message The error message.
         */
        explicit SettingsMenuError(const std::string &message) : std::runtime_error("\n\t" + message)
        {
        }

        /**
         * @brief Override of the what() method from std::exception.
         * @return The error message as a C-style string.
         */
        const char *what() const noexcept override
        {
            return _message.c_str();
        }

      private:
        std::string _message; ///> Error message
    };

    /**
     * @brief Class representing the settings menu of the game.
     */
    class SettingsMenu {
      public:
        /**
         * @brief Construct a new Settings Menu object.
         *
         * @param renderer Shared pointer to the renderer used for rendering the settings menu.
         * @param musicRegistry Shared pointer to the music registry.
         * @param soundRegistry Shared pointer to the sound registry.
         */
        explicit SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer,
            std::shared_ptr<MusicRegistry> musicRegistry, std::shared_ptr<SoundRegistry> soundRegistry);

        /**
         * @brief Update the settings menu state.
         * @param frame The current input frame.
         */
        void update(const InputFrame &frame);

        /**
         * @brief Render the settings menu.
         */
        void render() const;

        /**
         * @brief Called when entering the settings menu.
         */
        void onEnter();

        /**
         * @brief Check if the user wants to go back from the settings menu.
         *
         * @return true if the user wants to go back, false otherwise.
         */
        bool wantsBack() const noexcept;

        /**
         * @brief Check if the screen resolution has been changed.
         *
         * @return true if the resolution has been changed, false otherwise.
         */
        bool resolutionChanged() noexcept;

        /**
         * @brief Get the current screen resolution.
         *
         * @return The current screen resolution.
         */
        Graphics::Extent2u currentResolution() const noexcept;

        /**
         * @brief Handle resizing of the settings menu.
         */
        void layout();

      private:
        /**
         * @brief Handle input for the settings menu.
         * @param frame The current input frame.
         */
        void handleInput(const InputFrame &frame);

        std::shared_ptr<Graphics::IRenderer> _renderer; ///> Renderer used for rendering the settings menu
        std::shared_ptr<MusicRegistry> _musicRegistry;  ///> Music registry
        std::shared_ptr<SoundRegistry> _soundRegistry;  ///> Sound registry

        Graphics::TextureHandle _backgroundTexture; ///> Texture handle for the background image
        RenderCommand _backgroundCmd;               ///> Render command for the background image

        std::unique_ptr<UI::UIButton> _colorBlindMode; ///> Colorblind mode button
        std::unique_ptr<UI::UIButton> _colorBlindNext; ///> Next colorblind mode button
        std::unique_ptr<UI::UIButton> _left;           ///> Decrease resolution button
        std::unique_ptr<UI::UIButton> _right;          ///> Increase resolution button
        std::unique_ptr<UI::UIButton> _back;           ///> Back button
        std::unique_ptr<UI::UIButton> _resolution;     ///> Current resolution display button
        std::unique_ptr<UI::UIButton> _resolutionNext; ///> Next resolution button

        int _musicVolume = 50;    ///> Music volume level (0-100)
        int _sfxVolume = 50;      ///> SFX volume level (0-100)
        bool _musicMuted = false; ///> Music mute state
        bool _sfxMuted = false;   ///> SFX mute state

        std::unique_ptr<UIButton> _musicVolLabel; ///> Music volume display
        std::unique_ptr<UIButton> _musicVolUp;    ///> Increase music volume
        std::unique_ptr<UIButton> _musicVolDown;  ///> Decrease music volume
        std::unique_ptr<UIButton> _sfxVolLabel;   ///> SFX volume display
        std::unique_ptr<UIButton> _sfxVolUp;      ///> Increase SFX volume
        std::unique_ptr<UIButton> _sfxVolDown;    ///> Decrease SFX volume
        std::unique_ptr<UIButton> _muteMusic;     ///> Mute/unmute music
        std::unique_ptr<UIButton> _muteSFX;       ///> Mute/unmute SFX

        bool _backRequested = false; ///> Flag indicating if the user wants to go back

        /**
         * @brief List of available screen resolutions.
         */
        std::vector<Graphics::Extent2u> _resolutions{{900, 600}, {1280, 720}, {1920, 1080}};
        std::size_t _currentResolution = 1; ///> Index of the current screen resolution
        bool _resolutionChanged = false;    ///> Flag indicating if the resolution has been changed

        Graphics::ColorBlindMode _currentColorBlindMode = Graphics::ColorBlindMode::NONE; ///> Current colorblind mode
    };
} // namespace Engine
