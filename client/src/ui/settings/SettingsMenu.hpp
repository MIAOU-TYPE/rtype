/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SettingsMenu
*/

#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include "IRenderer.hpp"
#include "IText.hpp"
#include "InputState.hpp"
#include "RenderCommand.hpp"
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
         */
        explicit SettingsMenu(const std::shared_ptr<Graphics::IRenderer> &renderer);

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

        Graphics::TextureHandle _backgroundTexture; ///> Texture handle for the background image
        RenderCommand _backgroundCmd;               ///> Render command for the background image

        std::unique_ptr<UIButton> _audio;          ///> Audio settings button
        std::unique_ptr<UIButton> _colorBlindMode; ///> Colorblind mode button
        std::unique_ptr<UIButton> _colorBlindNext; ///> Next colorblind mode button
        std::unique_ptr<UIButton> _left;           ///> Decrease resolution button
        std::unique_ptr<UIButton> _right;          ///> Increase resolution button
        std::unique_ptr<UIButton> _back;           ///> Back button
        std::unique_ptr<UIButton> _resolution;     ///> Current resolution display button
        std::unique_ptr<UIButton> _resolutionNext; ///> Next resolution button

        bool _backRequested = false; ///> Flag indicating if the user wants to go back

        /**
         * @brief List of available screen resolutions.
         */
        std::vector<Graphics::Extent2u> _resolutions{{900, 600}, {1280, 720}, {1920, 1080}};
        std::size_t _currentResolution = 1; ///> Index of the current screen resolution
        bool _resolutionChanged = false;    ///> Flag indicating if the resolution has been changed

        /**
         * @brief Get the next colorblind mode in sequence.
         * @param m The current colorblind mode.
         * @return The next colorblind mode.
         */
        static Graphics::ColorBlindMode nextMode(Graphics::ColorBlindMode m);
        Graphics::ColorBlindMode _currentColorBlindMode = Graphics::ColorBlindMode::NONE; ///> Current colorblind mode
    };
} // namespace Engine
